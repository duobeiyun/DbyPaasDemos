module.exports = exports = function (str) {
  if (str === 'binding') {
    let result;
    try {
      result = require('../node_modules/ref/build/Release/binding.node');
      result.path = '../node_modules/ref/build/Release/binding.node';
    } catch (e) {
      try {
        result = require('../node_modules/dby-paas-electron-sdk/node_modules/ref/build/Release/binding.node');
        result.path = '../node_modules/dby-paas-electron-sdk/node_modules/ref/build/Release/binding.node';
      } catch (e) {

      }
    }
    return result;
  } else if (str === 'ffi_bindings.node') {
    let result;
    try {
      result = require('../node_modules/ffi/build/Release/ffi_bindings.node');
      result.path = '../node_modules/ffi/build/Release/ffi_bindings.node';
    } catch (e) {
      try {
        result = require('../node_modules/dby-paas-electron-sdk/node_modules/ffi/build/Release/ffi_bindings.node');
        result.path = '../node_modules/dby-paas-electron-sdk/node_modules/ref/ffi/Release/ffi_bindings.node';
      } catch (e) {}
    }
    return result;
  }
};
