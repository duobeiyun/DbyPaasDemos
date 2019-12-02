const path = require('path')
const webpackMerge = require('webpack-merge')
const _ = require('lodash')

let baseConfig = require('./webpack.config.base')

baseConfig = _.cloneDeep(baseConfig)
baseConfig.devServer = {}

const config = webpackMerge(baseConfig, {
  mode: process.env.NODE_ENV,
  entry: {
    index: './src/main/index.js',
  },
  output: {
    path: path.join(process.cwd(), './build/dev/'),
    filename: 'main.js',
  },
  target: 'electron-main',
  watch: true,
})

module.exports = config
