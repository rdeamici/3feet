//
//  FirebaseService.swift
//  treehacks2019
//
//  Created by Haejin Jo on 2/16/19.
//  Copyright © 2019 Haejin Jo. All rights reserved.
//

import Foundation
import Firebase
import FirebaseFirestore

class FirebaseService: UIViewController, UITableViewDelegate {
    
//    class func testingStuff() {
//
//        // Add a new document (incident) with a generated ID
//        var ref: DocumentReference? = nil
//
//        let delegate = UIApplication.shared.delegate as! AppDelegate
//
//        ref = delegate.db.collection("incidents").addDocument(data: [
//            "time": Date(),
//            "lat": "",
//            "long": ""
//        ]) { err in
//            if let err = err {
//                print("Error adding doccument: \(err)")
//            } else {
//                print("Document added with ID: \(ref!.documentID)")
//            }
//        }
//    }

    
    // Add a new document (incident) with a generated ID
    func saveIncident() {
        let delegate = UIApplication.shared.delegate as! AppDelegate

        var ref: DocumentReference? = nil
        ref = delegate.db.collection("incidents").addDocument(data: [
            "time": Date(),
            "lat": "",
            "long": ""
        ]) { err in
            if let err = err {
                print("Error adding doccument: \(err)")
            } else {
                print("Document added with ID: \(ref!.documentID)")
            }
        }
    }
    //ref = Database.database().reference()
}
