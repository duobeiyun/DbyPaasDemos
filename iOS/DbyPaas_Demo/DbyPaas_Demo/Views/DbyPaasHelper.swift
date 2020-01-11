//
//  DbyPaasHelper.swift
//  DbyPaas_Demo
//
//  Created by yxibng on 2019/10/30.
//  Copyright © 2019 yxibng. All rights reserved.
//

import Foundation
import MBProgressHUD


public extension MBProgressHUD {
    static func showMessage(msg: String, inView view: UIView?) -> Void {
        guard let loadingView = view else {
            return
        }
        runOnMainQueueWithoutDeadlocking {
            let hud = MBProgressHUD.showAdded(to: loadingView, animated: true)
            hud.detailsLabel.text = msg
            hud.graceTime = 0.5
            hud.removeFromSuperViewOnHide = true
            hud.hide(animated: false, afterDelay: 1.0)
        }
    }
    
    static func startLoading(withMessage msg: String, in view: UIView?) {
        
        guard let loadingView = view  else {
            return
        }
        
        runOnMainQueueWithoutDeadlocking {
            let hud = MBProgressHUD.showAdded(to: loadingView, animated: true)
            hud.detailsLabel.text = msg
            hud.graceTime = 0.5
            hud.removeFromSuperViewOnHide = true
        }
    }
    
    static func stopLoading(in view: UIView?) {
        guard let loadingView = view else {
            return
        }
        runOnMainQueueWithoutDeadlocking {
            MBProgressHUD.hide(for: loadingView, animated: false)
        }
    }
}
