//
//  ViewController.swift
//  treehacks2019
//
//  Created by Haejin Jo on 2/16/19.
//  Copyright © 2019 Haejin Jo. All rights reserved.
//

import UIKit
import ArcGIS
import HoundifySDK

class ViewController: UIViewController {
    
    var mapView: AGSMapView!
    var animationView: UIView?

    
    
    // ESRI
    private func setupMap() {
        self.mapView = AGSMapView()
        self.mapView.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(mapView)
        NSLayoutConstraint.activate([
            self.mapView.leadingAnchor.constraint(equalTo: self.view.leadingAnchor),
            self.mapView.trailingAnchor.constraint(equalTo: self.view.trailingAnchor),
            self.mapView.topAnchor.constraint(equalTo: self.view.topAnchor),
            self.mapView.bottomAnchor.constraint(equalTo:self.view.bottomAnchor)
        ])
        
        mapView.map = AGSMap(basemapType: .navigationVector, latitude: 34.09042, longitude: -118.71511, levelOfDetail: 10)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupMap()
        setupLocationDisplay()
        testAnimationView()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        // Observe HoundVoiceSearchHotPhrase to be notified of when the hot phrase is detected.
//        NotificationCenter.default.addObserver(self, selector: #selector(handle(houndVoiceSearchHotPhraseNotification:)), name: .HoundVoiceSearchHotPhrase, object: nil)
//
        startListeningForHotPhrase()
    }

    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        NotificationCenter.default.removeObserver(self)
    }
    
    // thx tech bro
    private func testAnimationView() {
        self.animationView = UIView()
        
        if let animationView = self.animationView {
            self.view.addSubview(animationView)
            animationView.backgroundColor = .red
            animationView.frame = CGRect(x: 0, y: 0, width: UIScreen.main.bounds.width, height: UIScreen.main.bounds.height)
            UIView.animate(withDuration: 5.0) {
                animationView.alpha = 0.0;
            }
        }
    }

    // ESRI
    private func setupLocationDisplay() {
        mapView.locationDisplay.autoPanMode = .compassNavigation
        mapView.locationDisplay.start { [weak self] (error:Error?) -> Void in
            if let error = error {
                self?.showAlert(withStatus: error.localizedDescription)
            }
        }
    }
    
    private func showAlert(withStatus: String) {
        let alertController = UIAlertController(title: "Alert", message:
            withStatus, preferredStyle: .alert)
        alertController.addAction(UIAlertAction(title: "Dismiss", style: .default, handler: nil))
        present(alertController, animated: true, completion: nil)
    }
    
    private func startListeningForHotPhrase() {
        
        // Houndify.presentListingViewController(...) will activate audio if necessary, but
        // if you wish to support beginning voice queries with a hot phrase, you will need to
        // explicitly start HoundVoiceSearch listening.
        
        HoundVoiceSearch.instance().startListening(completionHandler: { (error: Error?) in
            if let error = error {
                print(error.localizedDescription)
            } else {
                HoundVoiceSearch.instance().enableHotPhraseDetection = true
                self.activateVoiceSearch()
            }
        })
    }
    
    func activateVoiceSearch() {
        
        // Launch the houndify listening UI using presentListeningViewController(in:, from:, style:, requestInfo:, responseHandler:)
        Houndify.instance().presentListeningViewController(in: self, // used to be UIVC extnded tabBarController variable
                                                           from: mapView,
                                                           style: nil,
                                                           requestInfo: [:],
                                                           responseHandler:
            
            { (error: Error?, response: Any?, dictionary: [String : Any]?, requestInfo: [String : Any]?) in
                if let error = error as NSError? {
                    print("\(error.domain) \(error.code) \(error.localizedDescription)")
                } else if let dictionary = dictionary {
                    print("hi")
                    //print(JSONAttributedFormatter.attributedString(from: dictionary, style: nil))
                }
                
                if  let serverData = response as? HoundDataHoundServer,
                    let commandResult = serverData.allResults?.firstObject() as? HoundDataCommandResult,
                    let nativeData = commandResult["NativeData"]
                {
                    print("NativeData: \(nativeData)")
                }
                
                self.dismissSearch()
        }
        ) // end of presentListeningViewController
    }
    
    fileprivate func dismissSearch() {
        Houndify.instance().dismissListeningViewController(animated: true, completionHandler: nil)
    }
    

}
