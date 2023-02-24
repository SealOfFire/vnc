using System.ComponentModel.Design.Serialization;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Reflection;
using System.Runtime.InteropServices;
using X11;

namespace VNC
{
    internal static class Extent
    {
        public static void CopyToArray(this ushort value, byte[] destinationArray, int destinationIndex)
        {
            byte[] buffer = BitConverter.GetBytes(value);
            for (int i = 0; i< buffer.Length; i++)
            {
                destinationArray[destinationIndex+i] = buffer[buffer.Length -1 -i];
            }
        }

        public static void CopyToArray(this uint value, byte[] destinationArray, int destinationIndex)
        {
            byte[] buffer = BitConverter.GetBytes(value);
            for (int i = 0; i< buffer.Length; i++)
            {
                destinationArray[destinationIndex+i] = buffer[buffer.Length - 1-i];
            }
        }

        public static void CopyToArray(this int value, byte[] destinationArray, int destinationIndex)
        {
            byte[] buffer = BitConverter.GetBytes(value);
            for (int i = 0; i< buffer.Length; i++)
            {
                destinationArray[destinationIndex+i] = buffer[buffer.Length - 1-i];
            }
        }

        public static byte[] ReadBuffer(this byte[] buffer, int start, int length)
        {
            byte[] bytes = new byte[length];
            for (int i = 0; i< bytes.Length; i++)
            {
                bytes[bytes.Length -1-i] = buffer[start+i];
            }
            return bytes;
        }

        public static byte[] ReadBuffer(this Stream stream, int length)
        {
            byte[] buffer = new byte[length];
            int i = stream.Read(buffer, 0, buffer.Length);
            Array.Reverse(buffer);
            return buffer;
        }

        public static ushort ReadBuffer(this byte[] buffer, int start)
        {
            byte[] bytes = buffer.ReadBuffer(start, 2);
            return BitConverter.ToUInt16(bytes);
        }

        //public static string ReadString(this Stream stream, int length)
        //{ 
        //    stream.ReadBuffer(length);
        //}

        //public static byte[] WriteBuffer(this Stream stream, int count)
        //{
        //    byte[] buffer = new byte[count];
        //    stream.Read(buffer, 0, buffer.Length);
        //    Array.Reverse(buffer);
        //    return buffer;
        //}

        public static byte ReadU8(this Stream stream)
        {
            byte[] buffer = stream.ReadBuffer(1);
            return buffer[0];
        }

        public static ushort ReadU16(this Stream stream)
        {
            byte[] buffer = stream.ReadBuffer(2);
            return BitConverter.ToUInt16(buffer, 0);
        }

        public static uint ReadU32(this Stream stream)
        {
            byte[] buffer = stream.ReadBuffer(4);
            return BitConverter.ToUInt32(buffer, 0);
        }

        public static int ReadS32(this Stream stream)
        {
            byte[] buffer = stream.ReadBuffer(4);
            return BitConverter.ToInt32(buffer, 0);
        }

        /// <summary>
        /// // 指定位置的屏幕截图
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        public static Bitmap ScreenCapture(ushort x, ushort y, ushort width, ushort height)
        {
            try
            {
                Stopwatch t = Stopwatch.StartNew();
                Bitmap bitmap = new Bitmap(width, height);
                System.Drawing.Graphics g = Graphics.FromImage(bitmap);
                g.CopyFromScreen(x, y, 0, 0, new Size(width, height));

                t.Stop();
                Service.logger.Trace($"Screen capture done in:[{t.ElapsedMilliseconds}]ms");
                //Trace.WriteLine("Screen capture done in: " + t.ElapsedMilliseconds + "ms");
                return bitmap;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
                System.Threading.Thread.Sleep(200);
                try
                {
                    Bitmap bitmap = new Bitmap(width, height);
                    Graphics g = Graphics.FromImage(bitmap);
                    g.CopyFromScreen(x, y, 0, 0, new Size(width, height));
                }
                catch (Exception)
                {
                    return null;
                }
                return null;
            }
        }

        /// <summary>
        /// 截屏数据转成数组
        /// </summary>
        /// <param name="bitmap"></param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="w"></param>
        /// <param name="h"></param>
        /// <param name="pf"></param>
        /// <returns></returns>
        public static byte[] GrabPixels(Bitmap bitmap, int x, int y, int w, int h, PixelFormat pf)
        {
            //int[] array = new int[w * h];
            BitmapData bmpData = bitmap.LockBits(new Rectangle(x, y, w, h), ImageLockMode.ReadOnly, pf);

            // Get the address of the first line.
            IntPtr ptr = bmpData.Scan0;

            // Declare an array to hold the bytes of the bitmap.
            int bytes = Math.Abs(bmpData.Stride) * bitmap.Height;
            byte[] rgbValues = new byte[bytes];

            // Copy the RGB values into the array.
            System.Runtime.InteropServices.Marshal.Copy(ptr, rgbValues, 0, bytes);

            // Set every third value to 255. A 24bpp bitmap will look red.  
            //for (int counter = 2; counter < rgbValues.Length; counter += 3)
            //    rgbValues[counter] = 255;

            // Copy the RGB values back to the bitmap
            System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

            bitmap.UnlockBits(bmpData);
            return rgbValues;
        }
    }
}
