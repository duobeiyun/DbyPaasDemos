Pod::Spec.new do |spec|
    spec.name         = 'DbyPaas_iOS'
    spec.version      = '1.0.0-5'
    spec.license      = { :type => 'Copyright',:text => 'Copyright 2019 duobeiyun.com. All rights reserved.\n'}
    spec.homepage     = 'https://sail-doc.duobeiyun.net/'
    spec.authors      = { 'yaoxiaobing' => 'xiaobing.yao@duobei.com' }
    spec.summary      = 'DbyPaas iOS video SDK.'
    spec.source       = { :http => 'https://sdk.duobeiyun.net/ios/sdk/1.0.0-5/DbyPaas_iOS.framework.zip' }
    spec.vendored_frameworks = 'DbyPaas_iOS.framework'
    spec.requires_arc = true
    spec.platform = :ios, '9.0'
  end
