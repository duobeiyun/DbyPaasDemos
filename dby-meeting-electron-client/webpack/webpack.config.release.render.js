const path = require('path')
const fs = require('fs')
const HtmlWebpackPlugin = require('html-webpack-plugin')
const webpackMerge = require('webpack-merge')

const baseConfig = require('./webpack.config.base')

const config = webpackMerge(baseConfig, {
  mode: 'production',
  entry: {
    index: './src/render/index.js',
  },
  output: {
    path: path.join(process.cwd(), './build/release/'),
    filename: 'render.js',
  },
  target: 'electron-renderer',
  plugins: [
    new HtmlWebpackPlugin({
      inject: 'body',
      templateContent: fs.readFileSync('./public/index.html').toString(),
      filename: 'index.html',
    }),
  ],
})

module.exports = config
