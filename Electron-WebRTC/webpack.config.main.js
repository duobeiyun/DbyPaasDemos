const webpackMerge = require('webpack-merge')
const path = require('path')

const baseConfig = require('./webpack.config.base')

const config = webpackMerge(baseConfig, {
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
