

INCLUDEPATH += /usr/include/glib-2.0 \
               /usr/lib/x86_64-linux-gnu/glib-2.0/include \
               /usr/include/gtk-3.0 \
               /usr/include/gdk-pixbuf-2.0 \
               /usr/include/pango-1.0 \
               /usr/include/harfbuzz \
               /usr/include/cairo \
               /usr/include/atk-1.0


LIBS += -L"/lib/x86_64-linux-gnu/"             -lz              \
        -L"/usr/lib/x86_64-linux-gnu/glib-2.0" -lglib-2.0       \
        -L"/usr/lib/x86_64-linux-gnu"          -lgtk-3          \
        -L"/usr/lib/x86_64-linux-gnu"          -lgdk-3          \
        -L"/usr/lib/x86_64-linux-gnu"          -lX11            \
        -L"/usr/lib/x86_64-linux-gnu"          -lcairo          \
        -L"/usr/lib/x86_64-linux-gnu"          -lgdk_pixbuf-2.0 \
        -L"/usr/lib/x86_64-linux-gnu"          -lgio-2.0        \
        -L"/usr/lib/x86_64-linux-gnu"          -lgobject-2.0    \
        -L"/usr/lib/x86_64-linux-gnu"          -lgcrypt         \
        -L"/usr/lib/x86_64-linux-gnu"          -lgnutls

