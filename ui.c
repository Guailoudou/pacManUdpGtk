#include "head.h"
#define CELL_SIZE 10
// 全局变量
static GtkWidget *stack = NULL;
static GtkWidget *game_grid = NULL;
static GtkWidget *label_score = NULL;
static GtkWidget *label_time = NULL;
static GtkCssProvider *css_provider = NULL; // 用于管理动态样式
static int current_score = 0;
static int time_seconds = 0;
static guint timer_id = 0;

// 回调函数：创建房间
static void on_create_room_clicked(GtkButton *button, gpointer user_data) {
    g_print("正在创建房间...\n");
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_game");
}

// 回调函数：加入房间
static void on_join_room_clicked(GtkButton *button, gpointer user_data) {
    g_print("正在加入房间...\n");
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_game");
}

// 回调函数：方向控制
static void on_move_clicked(GtkButton *button, gpointer user_data) {
    const gchar *direction = (const gchar *)user_data;
    if(strcmp(direction,"上")==0)up();
    if(strcmp(direction,"下")==0)down();
    if(strcmp(direction,"左")==0)left();
    if(strcmp(direction,"右")==0)right();
    g_print("移动方向: %s\n", direction);
}

/// --- 核心绘图函数 ---
// 当 DrawingArea 需要重绘时调用
static gboolean on_cell_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    // 获取存储在 widget 数据中的颜色值
    guint *color_val = (guint *)user_data;
    
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    // 解析颜色
    GdkRGBA color;
    gchar *hex = g_strdup_printf("#%06x", *color_val);
    gdk_rgba_parse(&color, hex);
    g_free(hex);

    // 1. 设置绘图颜色
    cairo_set_source_rgba(cr, color.red, color.green, color.blue, 1.0);

    // 2. 绘制矩形并填充 (覆盖整个控件区域)
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    // 3. (可选) 绘制细微边框以区分格子，不需要可注释掉
    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.1); // 非常淡的黑色边框
    cairo_set_line_width(cr, 0.5);
    cairo_stroke(cr);

    return FALSE; 
}
void update_cell_color(int x, int y, guint color_hex) {
    if (!game_grid) return;

    // 1. 从 Grid 中获取指定位置的控件
    // gtk_grid_get_child_at 返回位于该位置的 widget
    GtkWidget *cell = gtk_grid_get_child_at(GTK_GRID(game_grid), x, y);

    if (!cell) {
        g_print("警告: 找不到坐标 (%d, %d) 的格子\n", x, y);
        return;
    }

    // 2. 获取绑定的颜色指针
    // 我们之前用 "cell_color" 作为 key 存储了 guint*
    guint *color_ptr = (guint *)g_object_get_data(G_OBJECT(cell), "cell_color");

    if (!color_ptr) {
        g_print("警告: 格子 (%d, %d) 没有关联颜色数据\n", x, y);
        return;
    }

    // 3. 修改颜色值
    *color_ptr = color_hex;
    // g_print("格子 (%d, %d) 颜色已更新为 #%06X\n", x, y, color_hex);

    // 4. 触发重绘
    // 这会让 GTK 在下一个空闲周期调用 on_cell_draw，从而显示新颜色
    gtk_widget_queue_draw(cell);
}

// 初始化 100x100 网格 (使用 GtkDrawingArea)
static void init_game_grid() {
    if (!game_grid) return;

    // 清空旧内容
    gtk_container_foreach(GTK_CONTAINER(game_grid), (GtkCallback)gtk_widget_destroy, NULL);

    g_print("正在生成 100x100 格子 (DrawingArea 模式)...\n");
    

    // 冻结更新以提升性能
    GdkWindow *gdk_win = gtk_widget_get_window(game_grid);
    if (gdk_win) gdk_window_freeze_updates(gdk_win);

    for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 50; x++) {
            // 1. 创建绘图区域控件
            GtkWidget *drawing_area = gtk_drawing_area_new();
            getview();
            // 2. 强制设定大小
            gtk_widget_set_size_request(drawing_area, CELL_SIZE, CELL_SIZE);

            // 3. 生成随机颜色并存入堆内存
            guint *color_val = g_malloc(sizeof(guint));
            if(view[x][y]==-1)
                *color_val = WALL_BLOCK;
            else if(view[x][y]==0)
                *color_val = AIR_BLOCK;
            else if(view[x][y]==1)
                *color_val = COIN_BLOCK;
            else if(view[x][y]==2)
                *color_val = HIM_BLOCK;
            else if(view[x][y]==3)
                *color_val = GHOST_BLOCK;

            // 4. 将颜色指针绑定到控件，并设置释放函数 (控件销毁时自动 free 颜色内存)
            g_object_set_data_full(G_OBJECT(drawing_area), "cell_color", color_val, g_free);

            // 5. 连接绘图信号
            g_signal_connect(drawing_area, "draw", G_CALLBACK(on_cell_draw), color_val);

            // 6. 添加到网格
            gtk_grid_attach(GTK_GRID(game_grid), drawing_area, x, y, 1, 1);
            
            // 7. 显示控件
            gtk_widget_show(drawing_area);
        }
    }

    if (gdk_win) gdk_window_thaw_updates(gdk_win);
    g_print("格子生成完毕。\n");
}

// 定时器回调
static gboolean update_game_info(gpointer data) {
    time_seconds++;
    current_score += 1; 

    int minutes = time_seconds / 60;
    int seconds = time_seconds % 60;

    gchar *time_str = g_strdup_printf("时间: %02d:%02d", minutes, seconds);
    gchar *score_str = g_strdup_printf("分数: %d", current_score);

    gtk_label_set_text(GTK_LABEL(label_time), time_str);
    gtk_label_set_text(GTK_LABEL(label_score), score_str);

    g_free(time_str);
    g_free(score_str);
    
    return G_SOURCE_CONTINUE;
}
// 回调函数
static gboolean tack_callback(gpointer data) {
    tickTask();
    
    // 返回 TRUE 表示继续调用，返回 FALSE 表示停止并销毁定时器
    return G_SOURCE_CONTINUE; 
    // 或者直接写 return TRUE;
}
// 页面切换监听
static void on_stack_notify_visible_child(GtkStack *stack, GParamSpec *pspec, gpointer user_data) {
    const gchar *name = gtk_stack_get_visible_child_name(stack);
    if (g_strcmp0(name, "page_game") == 0) {
        if (gtk_container_get_children(GTK_CONTAINER(game_grid)) == NULL) {
            init_game_grid();
            isrun=true;
        }
        if (timer_id == 0) {
            timer_id = g_timeout_add_seconds(1, update_game_info, NULL);
        }
        guint tick_timer_id = g_timeout_add(50, tack_callback, NULL);
    }
}

int cdrui_init(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    g_random_set_seed(time(NULL));

    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    
    if (!gtk_builder_add_from_file(builder, "game_interface.ui", &error)) {
        g_printerr("无法加载 UI 文件: %s\n", error->message);
        g_error_free(error);
        g_object_unref(builder);
        return 1;
    }

    stack = GTK_WIDGET(gtk_builder_get_object(builder, "app_stack"));
    game_grid = GTK_WIDGET(gtk_builder_get_object(builder, "game_grid_container"));
    label_score = GTK_WIDGET(gtk_builder_get_object(builder, "label_score"));
    label_time = GTK_WIDGET(gtk_builder_get_object(builder, "label_time"));

    GtkWidget *btn_create = GTK_WIDGET(gtk_builder_get_object(builder, "btn_create_room"));
    GtkWidget *btn_join = GTK_WIDGET(gtk_builder_get_object(builder, "btn_join_room"));

    g_signal_connect(btn_create, "clicked", G_CALLBACK(on_create_room_clicked), NULL);
    g_signal_connect(btn_join, "clicked", G_CALLBACK(on_join_room_clicked), NULL);

    g_signal_connect(gtk_builder_get_object(builder, "btn_up"), "clicked", G_CALLBACK(on_move_clicked), "上");
    g_signal_connect(gtk_builder_get_object(builder, "btn_down"), "clicked", G_CALLBACK(on_move_clicked), "下");
    g_signal_connect(gtk_builder_get_object(builder, "btn_left"), "clicked", G_CALLBACK(on_move_clicked), "左");
    g_signal_connect(gtk_builder_get_object(builder, "btn_right"), "clicked", G_CALLBACK(on_move_clicked), "右");

    g_signal_connect(stack, "notify::visible-child-name", G_CALLBACK(on_stack_notify_visible_child), NULL);

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_widget_show_all(window);

    gtk_main();

    if (timer_id > 0) g_source_remove(timer_id);
    if (css_provider) g_object_unref(css_provider);
    g_object_unref(builder);

    return 0;
}