import electron from 'electron';

import {
  isLocal,
} from '../utils/env'
import packageInfo from '../../package.json';

const { app } = electron;

const menuSetting = () => {
  const name = packageInfo.build.productName;
  const template = [
    {
      label: '编辑',
      submenu: [
        {
          label: '撤销',
          accelerator: 'CmdOrCtrl+Z',
          role: 'undo',
        }, {
          label: '重做',
          accelerator: 'Shift+CmdOrCtrl+Z',
          role: 'redo',
        }, {
          type: 'separator',
        }, {
          label: '剪切',
          accelerator: 'CmdOrCtrl+X',
          role: 'cut',
        }, {
          label: '复制',
          accelerator: 'CmdOrCtrl+C',
          role: 'copy',
        }, {
          label: '粘贴',
          accelerator: 'CmdOrCtrl+V',
          role: 'paste',
        }, {
          label: '粘贴并匹配格式',
          accelerator: 'Shift+CmdOrCtrl+V',
          role: 'pasteandmatchstyle',
        }, {
          label: '删除',
          accelerator: 'Shift+CmdOrCtrl+V',
          role: 'delete',
        }, {
          label: '全选',
          accelerator: 'CmdOrCtrl+A',
          role: 'selectall',
        },
      ],
    }, (
      isLocal === true ? {
        label: '查看',
        submenu: [
          {
            label: '切换开发者工具',
            accelerator: process.platform === 'darwin' ? 'Alt+Command+I' : 'Ctrl+Shift+I',
            click: (item, focusedWindow) => {
              if (focusedWindow) {
                focusedWindow.toggleDevTools();
              }
            },
          },
          {
            label: '重载',
            accelerator: 'CmdOrCtrl+R',
            click: (item, focusedWindow) => {
              if (focusedWindow) {
                focusedWindow.reload();
              }
            },
          }
        ],
      } : {}
    ),
    {
      label: '窗口',
      role: 'window',
      submenu: [
        {
          label: '最小化',
          accelerator: 'CmdOrCtrl+M',
          role: 'minimize',
        }, {
          label: '关闭',
          accelerator: 'CmdOrCtrl+W',
          role: 'close',
        }, {
          type: 'separator',
        }, {
          label: '重新打开窗口',
          accelerator: 'CmdOrCtrl+Shift+T',
          enabled: false,
          key: 'reopenMenuItem',
          click: () => {
            app.emit('activate');
          },
        },
      ],
    },
  ];
  if (process.platform === 'darwin') {
    template.unshift({
      label: name,
      submenu: [{
        label: `关于 ${name}`,
        role: 'about',
      },
      {
        type: 'separator',
      },
      {
        type: 'separator',
      },
      {
        label: `隐藏 ${name}`,
        accelerator: 'Command+H',
        role: 'hide',
      },
      {
        label: '隐藏其它',
        accelerator: 'Command+Alt+H',
        role: 'hideothers',
      },
      {
        label: '显示全部',
        role: 'unhide',
      },
      {
        type: 'separator',
      },
      {
        label: '退出',
        accelerator: 'Command+Q',
        click: () => {
          app.quit();
        },
      }],
    });
  }
  return template;
};

export default menuSetting;
