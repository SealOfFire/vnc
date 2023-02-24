﻿using System.Collections;
using System.Runtime.InteropServices;

namespace VNC
{
    /// <summary>
    /// 执行客户端发过来的鼠标键盘操作
    /// </summary>
    internal class Avatar
    {
        private const int MOUSEEVENTF_LEFTDOWN = 0x0002;
        private const int MOUSEEVENTF_LEFTUP = 0x0004;
        private const int MOUSEEVENTF_RIGHTDOWN = 0x0008;
        private const int MOUSEEVENTF_RIGHTUP = 0x0010;
        private const int MOUSEEVENTF_MIDDLEDOWN = 0x0020;
        private const int MOUSEEVENTF_MIDDLEUP = 0x0040;

        private const uint KEYEVENTF_KEYDOWN = 0x0000;
        private const uint KEYEVENTF_EXTENDEDKEY = 0x0001;
        private const uint KEYEVENTF_KEYUP = 0x0002;
        private static Dictionary<uint, byte> KeyMap = new Dictionary<uint, byte>() {
            { 0xff08, 0x08 }, // BackSpace
            { 0xff09, 0x09 }, // Tab
            { 0xff0d, 0x0d }, // Return or Enter
            { 0xff1b, 0x1b }, // Escape
            { 0xff63, 0x63 }, // Insert
            { 0xffff, 0xff }, // Delete
            { 0xff50, 0x50 }, // Home
            { 0xff51, 0x51 }, // Left
            { 0xff52, 0x52 }, // Up
            { 0xff53, 0x53 }, // Right
            { 0xff54, 0x54 }, // Down
            { 0xff55, 0x55 }, // Page Up
            { 0xff56, 0x56 }, // Page Down
            { 0xff57, 0x57 }, // End
            { 0xffbe, 0xbe }, // F1
            { 0xffbf, 0xbf }, // F2
            { 0xffc0, 0xc0 }, // F3
            { 0xffc1, 0xc1 }, // F4
            { 0xffc2, 0xc2 }, // F5
            { 0xffc3, 0xc3 }, // F6
            { 0xffc4, 0xc4 }, // F7
            { 0xffc5, 0xc5 }, // F8
            { 0xffc6, 0xc6 }, // F9
            { 0xffc7, 0xc7 }, // F10
            { 0xffc8, 0xc8 }, // F11
            { 0xffc9, 0xc9 }, // F12
            { 0xffe1, 0xA0 }, // Shift (left)
            { 0xffe2, 0xA1 }, // Shift (right)
            { 0xffe3, 0xA2 }, // Control (left)
            { 0xffe4, 0xA3}, // Control (right)
            { 0xffe7, 0xe7 }, // Meta (left)
            { 0xffe8, 0xe8 }, // Meta (right)
            { 0xffe9, 0xA4 }, // Alt (left)
            { 0xffea, 0xA5 }, // Alt (right)
            { 0x0041, 0x41 }, // A
            { 0x0061, 0x41 }, // a
            { 0x0042, 0x42 }, // B
            { 0x0062, 0x42 }, // b
            { 0x0043, 0x43 }, // C
            { 0x0063, 0x43 }, // c
            { 0x0044, 0x44 }, // D
            { 0x0064, 0x44 }, // d
            { 0x0045, 0x45 }, // E
            { 0x0065, 0x45 }, // e
            { 0x0046, 0x46 }, // F
            { 0x0066, 0x46 }, // f
            { 0x0047, 0x47 }, // G
            { 0x0067, 0x47 }, // g
            { 0x0048, 0x48 }, // H
            { 0x0068, 0x48 }, // h
            { 0x0049, 0x49 }, // I
            { 0x0069, 0x49 }, // i
            { 0x004A, 0x4A }, // J
            { 0x006A, 0x4A }, // j
            { 0x004B, 0x4B }, // K
            { 0x006B, 0x4B }, // k
            { 0x004C, 0x4C }, // L
            { 0x006C, 0x4C }, // l
            { 0x004D, 0x4D }, // M
            { 0x006D, 0x4D }, // m
            { 0x004E, 0x4E }, // N
            { 0x006E, 0x4E }, // n
            { 0x004F, 0x4F }, // O
            { 0x006F, 0x4F }, // o
            { 0x0050, 0x50 }, // P
            { 0x0070, 0x50 }, // p
            { 0x0051, 0x51 }, // Q
            { 0x0071, 0x51 }, // q
            { 0x0052, 0x52 }, // R
            { 0x0072, 0x52 }, // r
            { 0x0053, 0x53 }, // S
            { 0x0073, 0x53 }, // s
            { 0x0054, 0x54 }, // T
            { 0x0074, 0x54 }, // t
            { 0x0055, 0x55 }, // U
            { 0x0075, 0x55 }, // u
            { 0x0056, 0x56 }, // V
            { 0x0076, 0x56 }, // v
            { 0x0057, 0x57 }, // W
            { 0x0077, 0x57 }, // w
            { 0x0058, 0x58 }, // X
            { 0x0078, 0x58 }, // x
            { 0x0059, 0x59 }, // Y
            { 0x0079, 0x59 }, // y
            { 0x005A, 0x5A }, // Z
            { 0x007A, 0x5A }, // z
            { 0x0021, 0x31 }, // 1!
            { 0x0040, 0x32 }, // 2@
            { 0x0023, 0x33 },  // 3#
            { 0x0024, 0x34 }, // 4$
            { 0x0025, 0x35 }, // 5%
            { 0x005E, 0x36 }, // 6^
            { 0x0026, 0x37 }, // 7&
            { 0x002A, 0x38 }, // 8*
            { 0x0028, 0x39 }, // 9(
            { 0x0029, 0x30 }, // 0)
            { 0x002D, 0xBD }, // -
            { 0x005F, 0xBD }, // _
            { 0x002B, 0xBB }, // +
            { 0x003D, 0xBB }, // =
            { 0x0060, 0xC0 }, // `
            { 0x007E, 0xC0 }, // ~
            { 0x005B, 0xDB }, // [
            { 0x007B, 0xDB }, // {
            { 0x005D, 0xDD }, // ]
            { 0x007D, 0xDD }, // }
            { 0x005C, 0xDC }, // \
            { 0x007C, 0xDC }, // |
            { 0x003A, 0xBA },// :
            { 0x003B, 0xBA },// ;
            { 0x0027, 0xDE }, //'
            { 0x0022, 0xDE },// "
            { 0x003C, 0xBC }, // <
            { 0x002C, 0xBC }, //,
            { 0x003E, 0xBE },// >
            { 0x002E, 0xBE },// .
            { 0x002F, 0xBF }, // /
            { 0x003F, 0xBF }, // ?
        };

        private bool[] isPress = new bool[3] { false, false, false };

        /// <summary>
        /// 鼠标移动
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        private static extern bool SetCursorPos(int x, int y);

        [DllImport("user32.dll")]
        private static extern void keybd_event(byte bVk, byte bScan, uint dwFlags, IntPtr dwExtraInfo);

        /// <summary>
        /// 鼠标按下
        /// </summary>
        /// <param name="dwFlags"></param>
        /// <param name="dx"></param>
        /// <param name="dy"></param>
        /// <param name="dwData"></param>
        /// <param name="dwExtraInfo"></param>
        [DllImport("user32.dll")]
        private static extern void mouse_event(
         int dwFlags, // motion and click options
         int dx, // horizontal position or change
         int dy, // vertical position or change
         int dwData, // wheel movement
         IntPtr dwExtraInfo // application-defined information
     );

        /// <summary>
        /// 鼠标操作
        /// </summary>
        public void PointerEvent(byte buttonMask, ushort x, ushort y)
        {
            BitArray bitArray = new BitArray(new byte[] { buttonMask });

            for (int i = 0; i< this.isPress.Length; i++)
            {
                if (!this.isPress[i] && bitArray[i])
                {
                    // 按下按键
                    this.isPress[i]=true;
                    switch (i)
                    {
                        case 0:
                            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, IntPtr.Zero);
                            break;
                        case 1:
                            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, IntPtr.Zero);
                            break;
                        case 2:
                            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, IntPtr.Zero);
                            break;
                    }

                }
                if (this.isPress[i] && !bitArray[i])
                {
                    // 抬起按键
                    this.isPress[i]=false;
                    switch (i)
                    {
                        case 0:
                            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, IntPtr.Zero);
                            break;
                        case 1:
                            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, IntPtr.Zero);
                            break;
                        case 2:
                            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, IntPtr.Zero);
                            break;
                    }
                }
            }

            // TODO 鼠标滚轮

            // 查看按键
            // 案件 蒙板 1-8
            // 0 up
            // 1 down
            // 传统鼠标
            // 1 做
            // 2 中
            // 3 右
            // 滑轮鼠标
            // 4 滑轮向上
            // 5 滑轮向下
            // 6 滑轮向左
            // 7 滑轮向右

            // 设置鼠标位置
            SetCursorPos((int)x, (int)y);
            // X11.Xlib.XGrabPointer();

            // System.Windows.Forms.Cursor.Position = new System.Drawing.Point(x, y);
        }

        /// <summary>
        /// 按键操作
        /// </summary>
        /// <param name="downFlag">非0时按下。0时抬起</param>
        /// <param name="key"></param>
        public void KeyEvent(byte downFlag, uint key)
        {
            if (KeyMap.ContainsKey(key))
            {
                keybd_event(KeyMap[key], 0, downFlag == 0 ? KEYEVENTF_KEYUP : KEYEVENTF_KEYDOWN, IntPtr.Zero);
            }
            else
            {
                keybd_event((byte)key, 0, downFlag == 0 ? KEYEVENTF_KEYUP : KEYEVENTF_KEYDOWN, IntPtr.Zero);
            }

            //if (downFlag !=0)
            //{
            //    // 按键按下
            //    keybd_event(key, 0, 0, IntPtr.Zero);
            //}
            //else
            //{
            //    // 按键抬起
            //    keybd_event(key, 0, KEYEVENTF_KEYUP, IntPtr.Zero);
            //}
        }

    }
}
