const os = require('os')
const { exec } = require('child_process')

if (os.platform() === 'win32') {
  exec('yarn run cp:dll:win')
} else if (os.platform() === 'darwin') {
  exec('yarn run cp:dll:mac')
}
