import { BrowserWindow, Menu } from 'electron'
import is from 'electron-is'

import menuSetting from './menuSetting'
import pkgJson from '../../package'

function createBrowserWindow() {
  const options = {
    title: `${pkgJson.build.productName}@${pkgJson.version}`,
    width: 360,
    height: 490,
    useContentSize: true,
    center: true,
    resizable: true,
    movable: true,
    fullscreenable: false,
    enableLargerThanScreen: false,
    frame: true,
    transparent: false,
    alwaysOnTop: false,
    clickThrough: 'pointer-events',
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
  }

  const mainWindow = new BrowserWindow(options)

  if (is.windows()) {
    mainWindow.setMenuBarVisibility(false);
  } else {
    // Menu配置
    const menu = Menu.buildFromTemplate(menuSetting());
    Menu.setApplicationMenu(menu);
  }

  return mainWindow
}

export default createBrowserWindow
