//
//  ViewController.swift
//  Swift-NSTableView
//
//  Created by Mathieu Savy on 08/04/2015.
//  Copyright (c) 2015 Mathieu Savy. All rights reserved.
//

import Cocoa

class ViewController: NSViewController {
    @IBOutlet var myTableView: NSTableView!
//    private var data : [Dictionary<String, String>]! = defaultData
    @IBOutlet var name: NSTextField!
    @IBOutlet var priority: NSTextField!
    @IBOutlet var remainTime: NSTextField!

    var scheduler = Scheduler(Algorithm: Alg.FSCF)
    var currAlg = Alg.FSCF

    @IBAction func addProcess(sender: NSButton) {
        let priority = self.priority.stringValue == "" ? 5 : Int(self.priority.stringValue)!
        scheduler.AddProcess(Name: name.stringValue, remainTime: Int(remainTime.stringValue)!, Priority: priority)
        myTableView.reloadData()
        name.stringValue = ""
        self.priority.stringValue = ""
        remainTime.stringValue = ""
    }
    
    @IBAction func nextCycle(sender: NSButton) {
        scheduler.TurnAround()
        myTableView.reloadData()
    }
    
    @IBAction func changeAlg(sender: NSPopUpButton) {
        if sender.titleOfSelectedItem == "FCFS" {
            currAlg = .FSCF
            scheduler.SetAlgorithm(Algorithm: currAlg)
        } else {
            currAlg = .fixedPriority
            scheduler.SetAlgorithm(Algorithm: currAlg)
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    
    override var representedObject: AnyObject? {
        didSet {
            // Update the view, if already loaded.
        }
    }
}

extension ViewController: NSTableViewDataSource, NSTableViewDelegate {
    
    func numberOfRowsInTableView(tableView: NSTableView) -> Int {
        if let list = self.scheduler.list {
            return list.count
        } else {
            return 0
        }
    }
    
    func tableView(_ tableView: NSTableView, viewForTableColumn tableColumn: NSTableColumn?, row row: Int) -> NSView? {
        if let _ = self.scheduler.list {
            let process = scheduler.list![row]
            let identifier = tableColumn!.identifier
            let textField = tableView.makeViewWithIdentifier(identifier, owner: self) as! NSTextField
            switch identifier {
                case "ID" : textField.objectValue = process.indentifier
                case "remainTime" : textField.objectValue = String(process.remainTime)
                case "spawnTime": textField.objectValue = String(process.spawnTime)
                case "priority": textField.objectValue = String(process.GetPriority())
                case "state": textField.objectValue = String(process.GetState())
                default: break
            }
            return textField
        } else {
            return nil
        }
    }
}

































