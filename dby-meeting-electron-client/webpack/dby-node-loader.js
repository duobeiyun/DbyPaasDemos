const path = require('path');

module.exports = function(content) {
  console.log('---------- node file loader------ ', this.resourcePath)
  const filePath = './node/' + path.parse(this.resourcePath).base;
  this.emitFile(filePath, content);
  return `
    try {
      var path = require('path');
      global.process.dlopen(module, path.resolve(__dirname, '${filePath}'));
    } catch(e) {
      throw new Error('Cannot open: ' + path.resolve(__dirname, '${filePath}') + ' ' + e);
    }
  `;
};

module.exports.raw = true;
