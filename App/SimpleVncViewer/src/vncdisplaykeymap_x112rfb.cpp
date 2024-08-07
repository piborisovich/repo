static guint16 keymap_x112rfb[0x10000];
void init_keymap_x112rfb()
{
    static bool is_init_x112rfb = false;
    if (is_init_x112rfb) {
        return;
    }
    is_init_x112rfb = true;
    keymap_x112rfb[0x20] = 0x39; /* 32 (XK_space) => 57 via 57 (KEY_SPACE) */
    keymap_x112rfb[0x27] = 0x28;       /* 39 (XK_apostrophe) => 40 via 40 (KEY_APOSTROPHE) */
    keymap_x112rfb[0x2c] = 0x33;       /* 44 (XK_comma) => 51 via 51 (KEY_COMMA) */
    keymap_x112rfb[0x2d] = 0xc;        /* 45 (XK_minus) => 12 via 12 (KEY_MINUS) */
    keymap_x112rfb[0x2e] = 0x34;       /* 46 (XK_period) => 52 via 52 (KEY_DOT) */
    keymap_x112rfb[0x2f] = 0x35;       /* 47 (XK_slash) => 53 via 53 (KEY_SLASH) */
    keymap_x112rfb[0x30] = 0xb;        /* 48 (XK_0) => 11 via 11 (KEY_0) */
    keymap_x112rfb[0x31] = 0x2;        /* 49 (XK_1) => 2 via 2 (KEY_1) */
    keymap_x112rfb[0x32] = 0x3;        /* 50 (XK_2) => 3 via 3 (KEY_2) */
    keymap_x112rfb[0x33] = 0x4;        /* 51 (XK_3) => 4 via 4 (KEY_3) */
    keymap_x112rfb[0x34] = 0x5;        /* 52 (XK_4) => 5 via 5 (KEY_4) */
    keymap_x112rfb[0x35] = 0x6;        /* 53 (XK_5) => 6 via 6 (KEY_5) */
    keymap_x112rfb[0x36] = 0x7;        /* 54 (XK_6) => 7 via 7 (KEY_6) */
    keymap_x112rfb[0x37] = 0x8;        /* 55 (XK_7) => 8 via 8 (KEY_7) */
    keymap_x112rfb[0x38] = 0x9;        /* 56 (XK_8) => 9 via 9 (KEY_8) */
    keymap_x112rfb[0x39] = 0xa;        /* 57 (XK_9) => 10 via 10 (KEY_9) */
    keymap_x112rfb[0x3b] = 0x27;       /* 59 (XK_semicolon) => 39 via 39 (KEY_SEMICOLON) */
    keymap_x112rfb[0x3d] = 0xd;        /* 61 (XK_equal) => 13 via 13 (KEY_EQUAL) */
    keymap_x112rfb[0x41] = 0x1e;       /* 65 (XK_a) => 30 via 30 (KEY_A) */
    keymap_x112rfb[0x42] = 0x30;       /* 66 (XK_b) => 48 via 48 (KEY_B) */
    keymap_x112rfb[0x43] = 0x2e;       /* 67 (XK_c) => 46 via 46 (KEY_C) */
    keymap_x112rfb[0x44] = 0x20;       /* 68 (XK_d) => 32 via 32 (KEY_D) */
    keymap_x112rfb[0x45] = 0x12;       /* 69 (XK_e) => 18 via 18 (KEY_E) */
    keymap_x112rfb[0x46] = 0x21;       /* 70 (XK_f) => 33 via 33 (KEY_F) */
    keymap_x112rfb[0x47] = 0x22;       /* 71 (XK_g) => 34 via 34 (KEY_G) */
    keymap_x112rfb[0x48] = 0x23;       /* 72 (XK_h) => 35 via 35 (KEY_H) */
    keymap_x112rfb[0x49] = 0x17;       /* 73 (XK_i) => 23 via 23 (KEY_I) */
    keymap_x112rfb[0x4a] = 0x24;       /* 74 (XK_j) => 36 via 36 (KEY_J) */
    keymap_x112rfb[0x4b] = 0x25;       /* 75 (XK_K) => 37 via 37 (KEY_K) */
    keymap_x112rfb[0x4c] = 0x26;       /* 76 (XK_l) => 38 via 38 (KEY_L) */
    keymap_x112rfb[0x4d] = 0x32;       /* 77 (XK_m) => 50 via 50 (KEY_M) */
    keymap_x112rfb[0x4e] = 0x31;       /* 78 (XK_n) => 49 via 49 (KEY_N) */
    keymap_x112rfb[0x4f] = 0x18;       /* 79 (XK_o) => 24 via 24 (KEY_O) */
    keymap_x112rfb[0x50] = 0x19;       /* 80 (XK_p) => 25 via 25 (KEY_P) */
    keymap_x112rfb[0x51] = 0x10;       /* 81 (XK_q) => 16 via 16 (KEY_Q) */
    keymap_x112rfb[0x52] = 0x13;       /* 82 (XK_r) => 19 via 19 (KEY_R) */
    keymap_x112rfb[0x53] = 0x1f;       /* 83 (XK_s) => 31 via 31 (KEY_S) */
    keymap_x112rfb[0x54] = 0x14;       /* 84 (XK_t) => 20 via 20 (KEY_T) */
    keymap_x112rfb[0x55] = 0x16;       /* 85 (XK_u) => 22 via 22 (KEY_U) */
    keymap_x112rfb[0x56] = 0x2f;       /* 86 (XK_v) => 47 via 47 (KEY_V) */
    keymap_x112rfb[0x57] = 0x11;       /* 87 (XK_w) => 17 via 17 (KEY_W) */
    keymap_x112rfb[0x58] = 0x2d;       /* 88 (XK_x) => 45 via 45 (KEY_X) */
    keymap_x112rfb[0x59] = 0x15;       /* 89 (XK_y) => 21 via 21 (KEY_Y) */
    keymap_x112rfb[0x5a] = 0x2c;       /* 90 (XK_z) => 44 via 44 (KEY_Z) */
    keymap_x112rfb[0x5b] = 0x1a;       /* 91 (XK_bracketleft) => 26 via 26 (KEY_LEFTBRACE) */
    keymap_x112rfb[0x5c] = 0x2b;       /* 92 (XK_backslash) => 43 via 43 (KEY_BACKSLASH) */
    keymap_x112rfb[0x5d] = 0x1b;       /* 93 (XK_bracketright) => 27 via 27 (KEY_RIGHTBRACE) */
    keymap_x112rfb[0x60] = 0x29;       /* 96 (XK_grave) => 41 via 41 (KEY_GRAVE) */
    keymap_x112rfb[0x61] = 0x1e;       /* 97 (XK_a) => 30 via 30 (KEY_A) */
    keymap_x112rfb[0x62] = 0x30;       /* 98 (XK_b) => 48 via 48 (KEY_B) */
    keymap_x112rfb[0x63] = 0x2e;       /* 99 (XK_c) => 46 via 46 (KEY_C) */
    keymap_x112rfb[0x64] = 0x20;       /* 100 (XK_d) => 32 via 32 (KEY_D) */
    keymap_x112rfb[0x65] = 0x12;       /* 101 (XK_e) => 18 via 18 (KEY_E) */
    keymap_x112rfb[0x66] = 0x21;       /* 102 (XK_f) => 33 via 33 (KEY_F) */
    keymap_x112rfb[0x67] = 0x22;       /* 103 (XK_g) => 34 via 34 (KEY_G) */
    keymap_x112rfb[0x68] = 0x23;       /* 104 (XK_h) => 35 via 35 (KEY_H) */
    keymap_x112rfb[0x69] = 0x17;       /* 105 (XK_i) => 23 via 23 (KEY_I) */
    keymap_x112rfb[0x6a] = 0x24;       /* 106 (XK_j) => 36 via 36 (KEY_J) */
    keymap_x112rfb[0x6b] = 0x25;       /* 107 (XK_K) => 37 via 37 (KEY_K) */
    keymap_x112rfb[0x6c] = 0x26;       /* 108 (XK_l) => 38 via 38 (KEY_L) */
    keymap_x112rfb[0x6d] = 0x32;       /* 109 (XK_m) => 50 via 50 (KEY_M) */
    keymap_x112rfb[0x6e] = 0x31;       /* 110 (XK_n) => 49 via 49 (KEY_N) */
    keymap_x112rfb[0x6f] = 0x18;       /* 111 (XK_o) => 24 via 24 (KEY_O) */
    keymap_x112rfb[0x70] = 0x19;       /* 112 (XK_p) => 25 via 25 (KEY_P) */
    keymap_x112rfb[0x71] = 0x10;       /* 113 (XK_q) => 16 via 16 (KEY_Q) */
    keymap_x112rfb[0x72] = 0x13;       /* 114 (XK_r) => 19 via 19 (KEY_R) */
    keymap_x112rfb[0x73] = 0x1f;       /* 115 (XK_s) => 31 via 31 (KEY_S) */
    keymap_x112rfb[0x74] = 0x14;       /* 116 (XK_t) => 20 via 20 (KEY_T) */
    keymap_x112rfb[0x75] = 0x16;       /* 117 (XK_u) => 22 via 22 (KEY_U) */
    keymap_x112rfb[0x76] = 0x2f;       /* 118 (XK_v) => 47 via 47 (KEY_V) */
    keymap_x112rfb[0x77] = 0x11;       /* 119 (XK_w) => 17 via 17 (KEY_W) */
    keymap_x112rfb[0x78] = 0x2d;       /* 120 (XK_x) => 45 via 45 (KEY_X) */
    keymap_x112rfb[0x79] = 0x15;       /* 121 (XK_y) => 21 via 21 (KEY_Y) */
    keymap_x112rfb[0x7a] = 0x2c;       /* 122 (XK_z) => 44 via 44 (KEY_Z) */
    keymap_x112rfb[0xd7] = 0x37;       /* 215 (XK_multiply) => 55 via 55 (KEY_KPASTERISK) */
    keymap_x112rfb[0xff08] = 0xe;      /* 65288 (XK_BackSpace) => 14 via 14 (KEY_BACKSPACE) */
    keymap_x112rfb[0xff09] = 0xf;      /* 65289 (XK_Tab) => 15 via 15 (KEY_TAB) */
    keymap_x112rfb[0xff0d] = 0x1c;     /* 65293 (XK_Return) => 28 via 28 (KEY_ENTER) */
    keymap_x112rfb[0xff13] = 0xc6;     /* 65299 (XK_Pause) => 198 via 119 (KEY_PAUSE) */
    keymap_x112rfb[0xff14] = 0x46;     /* 65300 (XK_Scroll_Lock) => 70 via 70 (KEY_SCROLLLOCK) */
    keymap_x112rfb[0xff15] = 0x54;     /* 65301 (XK_Sys_Req) => 84 via 99 (KEY_SYSRQ) */
    keymap_x112rfb[0xff1b] = 0x1;      /* 65307 (XK_Escape) => 1 via 1 (KEY_ESC) */
    keymap_x112rfb[0xff50] = 0xc7;     /* 65360 (XK_Home) => 199 via 102 (KEY_HOME) */
    keymap_x112rfb[0xff51] = 0xcb;     /* 65361 (XK_Left) => 203 via 105 (KEY_LEFT) */
    keymap_x112rfb[0xff52] = 0xc8;     /* 65362 (XK_Up) => 200 via 103 (KEY_UP) */
    keymap_x112rfb[0xff53] = 0xcd;     /* 65363 (XK_Right) => 205 via 106 (KEY_RIGHT) */
    keymap_x112rfb[0xff54] = 0xd0;     /* 65364 (XK_Down) => 208 via 108 (KEY_DOWN) */
    keymap_x112rfb[0xff55] = 0xc9;     /* 65365 (XK_Page_Up) => 201 via 104 (KEY_PAGEUP) */
    keymap_x112rfb[0xff56] = 0xd1;     /* 65366 (XK_Page_Down) => 209 via 109 (KEY_PAGEDOWN) */
    keymap_x112rfb[0xff57] = 0xcf;     /* 65367 (XK_End) => 207 via 107 (KEY_END) */
    keymap_x112rfb[0xff63] = 0xd2;     /* 65379 (XK_Insert) => 210 via 110 (KEY_INSERT) */
    keymap_x112rfb[0xff6a] = 0xf5;     /* 65386 (XK_Help) => 245 via 138 (KEY_HELP) */
    keymap_x112rfb[0xff7f] = 0x45;     /* 65407 (XK_Num_Lock) => 69 via 69 (KEY_NUMLOCK) */
    keymap_x112rfb[0xff8d] = 0x9c;     /* 65421 (XK_KP_Enter) => 156 via 96 (KEY_KPENTER) */
    keymap_x112rfb[0xffab] = 0x4e;     /* 65451 (XK_KP_Add) => 78 via 78 (KEY_KPPLUS) */
    keymap_x112rfb[0xffac] = 0x5c;     /* 65452 (XK_KP_Separator) => 92 via 95 (KEY_KPJPCOMMA) */
    keymap_x112rfb[0xffad] = 0x4a;     /* 65453 (XK_KP_Subtract) => 74 via 74 (KEY_KPMINUS) */
    keymap_x112rfb[0xffae] = 0x53;     /* 65454 (XK_KP_Decimal) => 83 via 83 (KEY_KPDOT) */
    keymap_x112rfb[0xffaf] = 0xb5;     /* 65455 (XK_KP_Divide) => 181 via 98 (KEY_KPSLASH) */
    keymap_x112rfb[0xffb0] = 0x52;     /* 65456 (XK_KP_0) => 82 via 82 (KEY_KP0) */
    keymap_x112rfb[0xffb1] = 0x4f;     /* 65457 (XK_KP_1) => 79 via 79 (KEY_KP1) */
    keymap_x112rfb[0xffb2] = 0x50;     /* 65458 (XK_KP_2) => 80 via 80 (KEY_KP2) */
    keymap_x112rfb[0xffb3] = 0x51;     /* 65459 (XK_KP_3) => 81 via 81 (KEY_KP3) */
    keymap_x112rfb[0xffb4] = 0x4b;     /* 65460 (XK_KP_4) => 75 via 75 (KEY_KP4) */
    keymap_x112rfb[0xffb5] = 0x4c;     /* 65461 (XK_KP_5) => 76 via 76 (KEY_KP5) */
    keymap_x112rfb[0xffb6] = 0x4d;     /* 65462 (XK_KP_6) => 77 via 77 (KEY_KP6) */
    keymap_x112rfb[0xffb7] = 0x47;     /* 65463 (XK_KP_7) => 71 via 71 (KEY_KP7) */
    keymap_x112rfb[0xffb8] = 0x48;     /* 65464 (XK_KP_8) => 72 via 72 (KEY_KP8) */
    keymap_x112rfb[0xffb9] = 0x49;     /* 65465 (XK_KP_9) => 73 via 73 (KEY_KP9) */
    keymap_x112rfb[0xffbd] = 0x59;     /* 65469 (XK_KP_Equal) => 89 via 117 (KEY_KPEQUAL) */
    keymap_x112rfb[0xffbe] = 0x3b;     /* 65470 (XK_F1) => 59 via 59 (KEY_F1) */
    keymap_x112rfb[0xffbf] = 0x3c;     /* 65471 (XK_F2) => 60 via 60 (KEY_F2) */
    keymap_x112rfb[0xffc0] = 0x3d;     /* 65472 (XK_F3) => 61 via 61 (KEY_F3) */
    keymap_x112rfb[0xffc1] = 0x3e;     /* 65473 (XK_F4) => 62 via 62 (KEY_F4) */
    keymap_x112rfb[0xffc2] = 0x3f;     /* 65474 (XK_F5) => 63 via 63 (KEY_F5) */
    keymap_x112rfb[0xffc3] = 0x40;     /* 65475 (XK_F6) => 64 via 64 (KEY_F6) */
    keymap_x112rfb[0xffc4] = 0x41;     /* 65476 (XK_F7) => 65 via 65 (KEY_F7) */
    keymap_x112rfb[0xffc5] = 0x42;     /* 65477 (XK_F8) => 66 via 66 (KEY_F8) */
    keymap_x112rfb[0xffc6] = 0x43;     /* 65478 (XK_F9) => 67 via 67 (KEY_F9) */
    keymap_x112rfb[0xffc7] = 0x44;     /* 65479 (XK_F10) => 68 via 68 (KEY_F10) */
    keymap_x112rfb[0xffe1] = 0x2a;     /* 65505 (XK_Shift_L) => 42 via 42 (KEY_LEFTSHIFT) */
    keymap_x112rfb[0xffe2] = 0x36;     /* 65506 (XK_Shift_R) => 54 via 54 (KEY_RIGHTSHIFT) */
    keymap_x112rfb[0xffe3] = 0x1d;     /* 65507 (XK_Control_L) => 29 via 29 (KEY_LEFTCTRL) */
    keymap_x112rfb[0xffe4] = 0x9d;     /* 65508 (XK_Control_R) => 157 via 97 (KEY_RIGHTCTRL) */
    keymap_x112rfb[0xffe5] = 0x3a;     /* 65509 (XK_Caps_Lock) => 58 via 58 (KEY_CAPSLOCK) */
    keymap_x112rfb[0xffe7] = 0xdb;     /* 65511 (XK_Meta_L) => 219 via 125 (KEY_LEFTMETA) */
    keymap_x112rfb[0xffe8] = 0xdc;     /* 65512 (XK_Meta_R) => 220 via 126 (KEY_RIGHTMETA) */
    keymap_x112rfb[0xffe9] = 0x38;     /* 65513 (XK_Alt_L) => 56 via 56 (KEY_LEFTALT) */
    keymap_x112rfb[0xffea] = 0xb8;     /* 65514 (XK_Alt_R) => 184 via 100 (KEY_RIGHTALT) */
    keymap_x112rfb[0xffff] = 0xd3;     /* 65535 (XK_Delete) => 211 via 111 (KEY_DELETE) */
}
