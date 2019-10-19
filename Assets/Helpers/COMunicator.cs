using System;
using System.Collections;
using System.IO.Ports;
using UnityEngine;

namespace Helpers
{
    public static class COMunicator
    {
        private static SerialPort stream;

        static COMunicator()
        {
            stream = new SerialPort("COM3", 9600);
            stream.ReadTimeout = 50;
            stream.Open();
        }
        
        
        private static void WriteToArduino(string message) {
            stream.WriteLine(message);
            stream.BaseStream.Flush();
        }
        
        private static IEnumerator AsynchronousReadFromArduino(Action<string> callback, Action fail, float timeout) {
            DateTime initialTime = DateTime.Now;
            DateTime nowTime;
            TimeSpan diff = default(TimeSpan);
 
            string dataString = null;
 
            do {
                try {
                    dataString = stream.ReadLine();
                    //Debug.unityLogger.Log(dataString);
                }
                catch (TimeoutException) {
                    dataString = null;
                }
 
                if (dataString != null)
                {
                    callback(dataString);
                    yield break; // Terminates the Coroutine
                } else
                    yield return null; // Wait for next frame
 
                nowTime = DateTime.Now;
                diff = nowTime - initialTime;
 
            } while (diff.Milliseconds < timeout);
 
            if (fail != null)
                fail();
            yield return null;
        }

        public static IEnumerator COMunicate(String action, Action<String> callback, Action fail = null, float timemout = 1000)
        {
            WriteToArduino(action);
            return AsynchronousReadFromArduino(callback, fail, timemout);
        }
    }
}