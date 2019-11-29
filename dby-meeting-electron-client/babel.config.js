
module.exports = (api) => {
  api.cache.using(() => process.env.BABEL_ENV_TARGETS_BROWSERS)

  return {
    presets: [
      ['@babel/preset-env', {
        targets: {
          browsers: process.env.BABEL_ENV_TARGETS_BROWSERS || '> 0.25%',
        },
      }],
      '@babel/preset-react',
    ],
    plugins: [
      '@babel/plugin-transform-async-to-generator',
      '@babel/plugin-proposal-optional-chaining',
      '@babel/plugin-transform-object-assign',
      '@babel/plugin-proposal-export-namespace-from',
      ['@babel/plugin-proposal-decorators', { legacy: true }],
      ['@babel/plugin-proposal-class-properties', { loose: true }],
      'lodash',
      ['import', {
        libraryName: 'antd',
        libraryDirectory: 'es',
        style: 'css',
      }],
    ],
  }
}
