#region CmdMessenger - MIT - (c) 2013 Thijs Elenbaas.
/*
  CmdMessenger - library that provides command based messaging

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  Copyright 2013 - Thijs Elenbaas
*/
#endregion

using System;
using System.IO;
using CommandMessenger.Transport;
namespace CommandMessenger.Transport.File
{
    /// <summary>Fas
    /// Manager for serial port data
    /// </summary>
    public class FileTransport : ITransport
    {
        private const int BufferSize = 4096;

        private volatile bool _connected;

        private readonly AsyncWorker _worker;
        private readonly object _serialReadWriteLock = new object();
        private readonly object _readLock = new object();
        private readonly byte[] _readBuffer = new byte[BufferSize];
        private int _bufferFilled;

        private FileStream _arduino2pc;
        private FileStream _pc2arduino;

        public event EventHandler DataReceived;                                 // Event queue for all listeners interested in NewLinesReceived events.

        /// <summary> Gets or sets the current serial port settings. </summary>
        /// <value> The current serial settings. </value>

        public FileTransport()
        {
            _arduino2pc = 
                System.IO.File.Open(@"c:\temp\serial2pc.txt", FileMode.OpenOrCreate, FileAccess.Read, FileShare.ReadWrite);
            _pc2arduino =
                System.IO.File.Open(@"c:\temp\serial2arduino.txt", FileMode.OpenOrCreate, FileAccess.Write, FileShare.ReadWrite);
            _arduino2pc.Seek(0, SeekOrigin.End);
            _worker = new AsyncWorker(Poll);
			_worker.Name = "FileTransport";
        }

        private bool Poll()
        {
            var bytes = UpdateBuffer();
            if (bytes > 0 && DataReceived != null) DataReceived(this, EventArgs.Empty);

            // Return true as we always have work to do here. The delay is achieved by SerialPort.Read timeout.
            return true;
        }        

        /// <summary> Connects to a serial port defined through the current settings. </summary>
        /// <returns> true if it succeeds, false if it fails. </returns>
        public bool Connect()
        {
            if (IsConnected())
                throw new InvalidOperationException("File is already opened.");

            _connected = Open();
            if (_connected) _worker.Start();

            return _connected;
        }

        /// <summary> Query if the serial port is open. </summary>
        /// <returns> true if open, false if not. </returns>
        public bool IsConnected()
        {
            return _connected;
        }

        /// <summary> Stops listening to the serial port. </summary>
        /// <returns> true if it succeeds, false if it fails. </returns>
        public bool Disconnect()
        {
            bool result = Close();
            if (_connected)
            {
                _connected = false;
                _worker.Stop();
            }
            return result;
        }

        /// <summary> Writes a parameter to the serial port. </summary>
        /// <param name="buffer"> The buffer to write. </param>
        public void Write(byte[] buffer)
        {
            
            if (IsConnected())
            {
                try
                {
                    lock (_serialReadWriteLock)
                    {
                        _pc2arduino.Write(buffer, 0, buffer.Length);
                        _pc2arduino.Flush();
                    }
                }
                catch (TimeoutException)
                {
                    // Timeout (expected)
                }
                catch
                {
                    Disconnect();
                }
            }
        }

        /// <summary> Reads the serial buffer into the string buffer. </summary>
        public byte[] Read()
        {
            if (IsConnected())
            {
                byte[] buffer;
                lock (_readLock)
                {
                    buffer = new byte[_bufferFilled];
                    Array.Copy(_readBuffer, buffer, _bufferFilled);
                    _bufferFilled = 0;
                }
                return buffer;
            }

            return new byte[0];
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary> Opens the serial port. </summary>
        /// <returns> true if it succeeds, false if it fails. </returns>
        private bool Open()
        {
            return true;
        }

        /// <summary> Closes the serial port. </summary>
        /// <returns> true if it succeeds, false if it fails. </returns>
        private bool Close()
        {
            if (!IsConnected()) return false;
            return true;
        }

        /// <summary> Queries if a current port exists. </summary>
        /// <returns> true if it succeeds, false if it fails. </returns>

        private int UpdateBuffer()
        {
            if (IsConnected())
            {
                try
                {
                    lock (_readLock)
                    {
                        var nbrDataRead = _arduino2pc.Read(_readBuffer, _bufferFilled, (BufferSize - _bufferFilled));
                        _bufferFilled += nbrDataRead;
                    }
                    return _bufferFilled;
                }
                catch (TimeoutException)
                {
                    // Timeout (expected)
                }
                catch (Exception)
                {
                    Disconnect();
                }
            }

            return 0;
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                Disconnect();
                _pc2arduino.Dispose();
                _arduino2pc.Dispose();
            }
        }
    }
}