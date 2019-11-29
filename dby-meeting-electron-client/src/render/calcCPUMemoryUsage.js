import * as si from 'systeminformation'

import pkgJson from '../../package'
import { isLocal } from '../utils/env'

import store from './store'

let totalMemory = 0
const productName = isLocal ? 'Electron' : pkgJson.build.productName

si.mem().then(({ total }) => {
  totalMemory = total / 1024 / 1024
})

function getAppCpuAndMemUsage() {
  return new Promise((resolve) => {
    si.processes().then((processes) => {
      return processes.list.filter((info) => {
        return info.name.indexOf(productName) !== -1 || info.name.indexOf('M-eM-$M^ZM-hM-4M^M-dM-:M^QM-dM') !== -1
      })
    }).then((processes) => {
      return processes.reduce((r, v) => {
        r.cpu += v.pcpu
        r.mem += v.pmem
        return r
      }, { cpu: 0, mem: 0 })
    }).then(resolve)
  })
}

setInterval(() => {
  getAppCpuAndMemUsage().then((usage) => {
    store.systemStatus.update({
      cpuUsage: parseInt(usage.cpu, 10),
      memoryUsage: parseInt(usage.mem * totalMemory / 100, 10), // 单位是 MB
    })
  })
}, 2000)
