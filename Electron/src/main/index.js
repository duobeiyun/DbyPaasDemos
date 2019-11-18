import url from 'url'
import path from 'path'
import { app, BrowserWindow, screen } from 'electron'

import 'dby-paas-electron-sdk/main'

import pkgJson from '../../package'

let mainWindow

app.commandLine.appendSwitch('max-active-webgl-contexts', '32')

app.on('ready', () => {
  const displayWorkAreaSize = screen.getAllDisplays()[0].workArea
  mainWindow = new BrowserWindow({
    title: `${pkgJson.name}@${pkgJson.version}`,
    width: parseInt(displayWorkAreaSize.width * 0.85, 10),
    height: parseInt(displayWorkAreaSize.height * 0.85, 10),
    useContentSize: true,
    center: true,
    resizable: true,
    movable: true,
    fullscreenable: false,
    enableLargerThanScreen: false,
    frame: true,
    transparent: false,
    alwaysOnTop: false,
    acceptFirstMouse: true,
    hasShadow: false,
    webPreferences: {
      devTools: true,
      webSecurity: false,
      plugins: true,
      experimentalFeatures: true,
      experimentalCanvasFeatures: true,
      minimumFontSize: 10,
    },
  })

  mainWindow.loadURL(url.format({
    protocol: 'file:',
    pathname: path.join(process.cwd(), './build/dev/index.html'),
  }))

  mainWindow.webContents.openDevTools({ mode: 'undocked' })

})
