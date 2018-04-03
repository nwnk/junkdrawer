/* display animated cursor */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<X11/keysymdef.h>
#include<X11/extensions/XInput.h>
#include<X11/extensions/Xrender.h>
#include<X11/cursorfont.h>
#include<assert.h>

static Display* dpy;

int main(int argc, char** argv)
{
    Window win;
    XSetWindowAttributes attr;
    GC gc;
    XGCValues gcvalues;
    Cursor* cursors;
    int num_cursors = 4; /* even number! */
    int i;
    Cursor ac1, ac2;
    XAnimCursor anim1[num_cursors/2], 
                anim2[num_cursors/2];

    int x = 0 , y = 0;

    if (argc >= 3) 
    {
        x = atoi(argv[1]);
        y = atoi(argv[2]);
        printf("Putting window at %d/%d\n", x, y);
    }

    dpy = XOpenDisplay(NULL);
    assert(dpy != NULL);

    win = XCreateSimpleWindow(dpy, RootWindow(dpy, 0), x, y,
            200, 200, 0, 0,
            WhitePixel(dpy, 0));
    assert(win);

    attr.event_mask = ExposureMask | StructureNotifyMask;

    XChangeWindowAttributes(dpy, win, CWEventMask, &attr);
    XMapWindow(dpy, win);
    XFlush(dpy);

    gc = XCreateGC(dpy, win, 0, &gcvalues);
    assert(gc != 0);
    XFlush(dpy);

    cursors = (Cursor*)calloc(sizeof(Cursor), num_cursors);

    for (i = 0; i < num_cursors; i++)
    {
        cursors[i] = XCreateFontCursor(dpy, i * 2);
        assert(cursors[i]);
    }

    for (i = 0; i < num_cursors/2; i++)
    {
        anim1[i].cursor = cursors[i];
        anim1[i].delay = 800;
        anim2[i].cursor = cursors[num_cursors/2 + i];
        anim2[i].delay = 200;
    }

    ac1 = XRenderCreateAnimCursor(dpy, num_cursors/2, anim1);
    ac2 = XRenderCreateAnimCursor(dpy, num_cursors/2, anim2);
    XDefineCursor(dpy, win, ac1);
    XDefineCursor(dpy, win, ac1);

    XWarpPointer(dpy, None, win, 0, 0, 0, 0, 100, 100);

    while(1)
    {
        XEvent e;
        XNextEvent(dpy, &e);

        switch(e.type)
        {
            case Expose:
                printf("expose\n");
                break;
        }

    }
    XCloseDisplay(dpy);
    return 0;
}
