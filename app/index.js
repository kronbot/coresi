const { app, BrowserWindow, ipcMain } = require('electron');
const fs = require('fs');
const path = require('path');
const SerialPort = require('serialport');
//const raspi = require('raspi');
//const Serial = require('raspi-serial').Serial;

let pathIndex = 0;

//const serial = new Serial({
//    portId: '/dev/ttyUSB0',
//    baudRate: 115200
//});

function createWindow() {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: true,
      preload: path.join(__dirname, './preload.js')
    },
    autoHideMenuBar: true
  });
  
  ipcMain.on('getImagePath', (event) => {
    const files = fs.readdirSync('./ads');
    
    event.sender.send('filePath', files[pathIndex++ % files.length]);    
  });

  ipcMain.on('sendCommand', (event, data) => {
    console.log(data);
    serial.write(data);
  });

  win.webContents.openDevTools();

  win.loadFile('index.html');
}

app.whenReady().then(createWindow);
