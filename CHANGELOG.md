#Joeization 2016/3/14
    add:
        class ai
    change:
        void view::drop to bool view::drop
            -if man drop is valid, then ai can drop
    bugs:
        after invalid drop, button wont bounce up

willyc20 2016/3/17
    add:
        void view::create_top_info
        void view::change_top_info
        void view::message_alert
        void view::ending_handler
    change:
        void view::change_board
            -add piece color

#Joeization 2016/3/17
    add:
        class train
        ai compute map value (very weak now)
    change:
        class view
            -add bool show to resolve trainning
        bool view::drop
            -and cnt to check valid
