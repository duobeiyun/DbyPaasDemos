const path = require('path')
const TerserPlugin = require('terser-webpack-plugin')

const webpack = require('webpack')

const isProd = process.env.NODE_ENV === 'production'

const config = {
  mode: 'development',
  module: {
    rules: [
      {
        test: /(\.js(x?)$)/,
        include: [
          /.*?dby-paas-electron-sdk.*?/,
          path.resolve(__dirname, './src'),
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
              // singleton: true,
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
            loader: 'dby-node-file-loader'
          }
        ]
      }
    ],
  },
  resolve: {
    extensions: ['.js', '.jsx', '.ts', '.tsx', '.json', '.less', '.css', '.node'],
  },
  target: 'web',
  plugins: [
    new webpack.DefinePlugin({
      'process.env.NODE_ENV': JSON.stringify(process.env.NODE_ENV) || "'development'",
    }),
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
}

module.exports = config
