const path = require('path')
const webpackMerge = require('webpack-merge')
const _ = require('lodash')

const baseConfig = require('./webpack.config.base')

const config = webpackMerge(baseConfig, {
  mode: 'production',
  entry: {
    index: './src/main/index.js',
  },
  output: {
    path: path.join(process.cwd(), './build/release/'),
    filename: 'main.js',
  },
  target: 'electron-main',
})

module.exports = config
