import 'babel-polyfill'
import url from 'url'
import path from 'path'
import { app, globalShortcut, ipcMain } from 'electron'
import is from 'electron-is'

import * as env from '../utils/env'
import { IPC_EVENT} from '../consts'
import '../utils/log'

import createBrowserWindow from './createBrowserWindow'
import 'dby-paas-electron-sdk/main'

import './crashReporter'

const { isLocal } = env

let mainWindow

app.commandLine.appendSwitch('max-active-webgl-contexts', '32')

app.on('ready', () => {
  mainWindow = createBrowserWindow()
  let pageFilePath
  if (isLocal) {
    pageFilePath = path.join(process.cwd(), './build/dev/index.html')
  } else {
    pageFilePath = path.join(app.getAppPath(), './build/release/index.html')
  }
  mainWindow.loadURL(url.format({
    protocol: 'file:',
    pathname: pageFilePath,
  }))


  let enableQuit = true
  mainWindow.on('close', (e) => {
    Log.info('****mainWindow on close', enableQuit)
    if (enableQuit === false) {
      mainWindow.webContents.send(IPC_EVENT.BEFORE_QUIT)
      e.preventDefault()
    } else {
      app.exit(0)
    }
  })
  ipcMain.on(IPC_EVENT.DISABLE_QUIT, () => {
    Log.info('*********main DISABLE_QUIT')
    enableQuit = false
  })
  ipcMain.on(IPC_EVENT.APP_QUIT, () => {
    Log.info('*********main APP_QUIT get')
    enableQuit = true
    app.exit(0)
  })


  mainWindow.on('window-all-closed', () => {
    Log.info('****mainWindow on close')
    app.exit(0)
  })

  if (env.isLocal) {
    mainWindow.webContents.openDevTools({ mode: 'undocked' })
  }

  // 打开调试工具
  globalShortcut.register(is.macOS() ? 'Alt+Command+K' : 'Ctrl+Shift+K', () => {
    mainWindow.webContents.openDevTools()
  })
  // 显示调试信息
  let showDebugInfo = false
  globalShortcut.register(is.macOS() ? 'Alt+Command+J' : 'Ctrl+Shift+J', () => {
    showDebugInfo = !showDebugInfo
    mainWindow.webContents.send(IPC_EVENT.TOGGLE_SHOW_DEBUG_INFO, showDebugInfo)
  })
})
