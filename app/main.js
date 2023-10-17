const { app, BrowserWindow } = require('electron')

const createWindow = () => {
    const win = new BrowserWindow({
        show: false
    });

    win.setMenu(null);

    win.setFullScreen(true);
    win.show();

    win.loadFile('index.html');
}

app.whenReady().then(() => {
    createWindow();
});