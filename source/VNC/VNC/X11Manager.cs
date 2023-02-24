using NLog.Fluent;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using X11;

namespace VNC
{
    public class X11Manager
    {
        private IntPtr display;
        private Window root;

        public XErrorHandlerDelegate OnError;

        [DllImport("libX11.so.6")]
        public static extern uint XDisplayWidth(IntPtr display, int screen_number);

        [DllImport("libX11.so.6")]
        public static extern uint XDisplayHeight(IntPtr display, int screen_number);

        [DllImport("libX11.so.6")]
        public static extern XImage XGetImage(IntPtr display, Window drawable, int x, int y, uint width, uint height, ulong plane_mask, PixmapFormat format);

        public X11Manager()
        {
            Service.logger.Info("初始化x11");

            this.display = Xlib.XOpenDisplay(null);
            if (display == IntPtr.Zero)
            {
                Service.logger.Error("Unable to open the default X display");
                Environment.Exit(1);
            }

            this.root = Xlib.XDefaultRootWindow(display);
            OnError = this.ErrorHandler;

            Xlib.XSetErrorHandler(OnError);

            /*
            // This will trigger a bad access error if another window manager is already running
            Xlib.XSelectInput(this.display, this.root,
                EventMask.SubstructureRedirectMask | EventMask.SubstructureNotifyMask |
                EventMask.ButtonPressMask | EventMask.KeyPressMask);

            Xlib.XSync(this.display, false);
            */
            int screen_num = Xlib.XDefaultScreen(this.display);
            Service.logger.Debug($"default screen_num:{screen_num}");

            XWindowAttributes gwa;
            Xlib.XGetWindowAttributes(display, root, out gwa);
            uint screen_width = gwa.width;
            uint screen_height = gwa.height;
            //uint screen_width = XDisplayWidth(display, screen_num);
            //uint screen_height = XDisplayHeight(display, screen_num);
            Service.logger.Debug($"screen_width:{screen_width},screen_height:{screen_height}");

            //Window disp_win = Xlib.XCreateSimpleWindow(display, root, 0, 0,
            //    screen_width, screen_height, 1, 0, 0);
            //if (disp_win !=0)
            //{

            //}

            //Xlib.XMapWindow(display, disp_win);
            ulong plane_mask = 0;
            //XImage image = 
            var img = XGetImage(display, root, 0, 0, screen_width, screen_height,
                ~plane_mask, PixmapFormat.ZPixmap);

            // Xlib.XPutInamge
            int debug = 0;
            debug++;

        }

        public int ErrorHandler(IntPtr display, ref XErrorEvent ev)
        {
            if (ev.error_code == 10) // BadAccess, i.e. another window manager has already claimed those privileges.
            {
                Service.logger.Error("X11 denied access to window manager resources - another window manager is already running");
                Environment.Exit(1);
            }

            // Other runtime errors and warnings.
            var description = Marshal.AllocHGlobal(1024);
            Xlib.XGetErrorText(this.display, ev.error_code, description, 1024);
            var desc = Marshal.PtrToStringAnsi(description);
            Service.logger.Warn($"X11 Error: {desc}");
            Marshal.FreeHGlobal(description);
            return 0;
        }
    }
}
