
module.exports = (api) => {
  api.cache(true)

  return {
    presets: [
      ['@babel/preset-env', {
        targets: {
          browsers: 'Chrome 56',
        },
      }],
      '@babel/preset-react',
    ],
    plugins: [
      '@babel/plugin-transform-async-to-generator',
      '@babel/plugin-transform-object-assign',
      '@babel/plugin-proposal-export-namespace-from',
      ['@babel/plugin-proposal-decorators', { legacy: true }],
      ['@babel/plugin-proposal-class-properties', { loose: true }],
      ['import', {
        libraryName: 'antd',
        libraryDirectory: 'es',
        style: 'css',
      }],
    ],
  }
}
