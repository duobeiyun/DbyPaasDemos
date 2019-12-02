const path = require('path')
const TerserPlugin = require('terser-webpack-plugin')

const webpack = require('webpack')

const isProd = process.env.NODE_ENV === 'production'

const config = {
  module: {
    rules: [
      {
        test: /\.worker\.js$/,
        use: {
          loader: 'raw-loader',
        },
      },
      {
        test: /(\.js(x?)$)/,
        include: [
          /.*?dby-paas-electron-sdk.*?/,
          path.resolve(__dirname, '../src'),
        ],
        // exclude: [/node_modules/],
        use: ['babel-loader'],
      },
      {
        test: /\.less$/,
        use: [
          {
            loader: 'style-loader',
            options: {
              singleton: true,
            },
          },
          {
            loader: 'css-loader',
            options: {
              modules: false,
            },
          },
          {
            loader: 'less-loader',
            options: {
              relativeUrls: true,
            },
          },
        ],
      },
      {
        test: /\.css$/,
        use: [
          {
            loader: 'style-loader',
          },
          {
            loader: 'css-loader',
          },
        ],
      },
      {
        test: /\.html$/,
        use: {
          loader: 'html-loader',
          options: {},
        },
      },
      {
        test: /\.(png|jpe?g|svg|gif|mp3|eot|ttf|woff)$/,
        use: {
          loader: 'file-loader',
          options: {
            name: 'static/media/[name].[hash:8].[ext]',
          },
        },
      },
      {
        test: /\.node$/,
        use: [
          {
            // loader: path.resolve('./webpack/dby-node-loader.js')
            loader: 'dby-node-file-loader'
          }
        ]
      }
    ],
  },
  resolve: {
    alias: {
      ROOT: path.resolve(__dirname), // 项目的根目录
      '~': path.resolve(__dirname), // 项目的根目录
      '#': path.resolve(process.cwd()), // 某个 package 的根目录/**/
    },
    extensions: ['.js', '.jsx', '.ts', '.tsx', '.json', '.less', '.css', '.node'],
  },
  target: 'web',
  plugins: [
    new webpack.DefinePlugin({ 'global.GENTLY': false }),
    new webpack.DefinePlugin({
      'process.env.NODE_ENV': JSON.stringify(process.env.NODE_ENV) || "'development'",
      'process.env.API_ENV': JSON.stringify(process.env.API_ENV) || "'api'", // 用来区分测试环境(包括本地测试与线上测试,值为api_dev)与正式环境(值为api)
    }),
    new webpack.ProvidePlugin({
      Log: [path.join(__dirname, '../src/utils/Log'), 'default'],
    }),
    new webpack.NormalModuleReplacementPlugin(/^bindings$/, require.resolve('./bindings')),
  ],
  devtool: false,
  optimization: {
    minimize: isProd === true,
    minimizer: [
      new TerserPlugin({
        terserOptions: {
          output: {
            comments: false,
          },
        },
      }),
    ],
  },
  node: {
    net: 'empty',
    tls: 'empty',
    fs: 'empty',
    __dirname: false,
    __filename: false,
  },
  devServer: {
    contentBase: path.join(process.cwd(), 'public'),
    compress: true,
    host: 'localhost',
    port: 9001,
    https: true,
    open: false,
    proxy: {
      '/collect': {
        target: 'http://data-center.duobeiyun.com',
        changeOrigin: true,
      },
      '/collect-event': {
        target: 'http://data-center.duobeiyun.com',
        changeOrigin: true,
      },
      '/api/**': {
        target: 'https://api_dev.duobeiyun.com',
        changeOrigin: true,
      },
      '/room/slides': {
        target: 'https://api_dev.duobeiyun.com',
        changeOrigin: true,
      },
      // 文件管理模块获取文件列表
      '/slides/**': {
        target: 'https://api_dev.duobeiyun.com',
        changeOrigin: true,
      },
      '/duobei-client/status': {
        target: 'https://log.duobeiyun.com',
        changeOrigin: true,
      },
      // 动态课件Container
      '/dcw/**': {
        target: 'https://api_dev.duobeiyun.com',
        changeOrigin: true,
      },
      '/slides': {
        target: 'https://api_dev.duobeiyun.com',
        changeOrigin: true,
      },
      '/duobei-client/ipinfo': {
        target: 'https://ipinfo.duobeiyun.com',
        changeOrigin: true,
      },
      '/stat-weixin': {
        target: 'https://stat-weixin.duobeiyun.com',
        changeOrigin: true,
      },
      '/api/webrtcserver/list': {
        target: 'https://api_dev.duobeiyun.com',
        changeOrigin: true,
      },
    },
  },
}

module.exports = config
