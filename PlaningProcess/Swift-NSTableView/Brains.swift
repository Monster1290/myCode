//
//  Brains.swift
//  Planing Process
//
//  Created by Александр on 29.10.15.
//  Copyright © 2015 Alexander Besedin. All rights reserved.
//

import Foundation

enum State {
    case running
    case ready
    case terminated
}

enum Alg {
    case FSCF
    case fixedPriority
}

class Process {
    
    var indentifier: String
    var spawnTime: Int
    var remainTime: Int
    var waitTime = 0
    var priority = 5
    var state = State.ready
    var terminateTime = 0
    
    init(indentifier: String, spawnTime : Int, remainTime: Int, priority: Int) {
        self.indentifier = indentifier
        self.spawnTime = spawnTime
        self.remainTime = remainTime
        self.priority = priority
    }
    
    func SetState(state sT: State) {
        state = sT
        waitTime = 0
    }
    
    func GetWaitTime() -> Int {return waitTime}
    func GetState() -> State {return state}
    func GetRemaining() -> Int {return remainTime}
    func GetPriority() -> Int {return priority}
    
    func Terminate(time t: Int) {
        remainTime = 0
        terminateTime = t
        SetState(state: .terminated)
    }
    func Stop(time t: Int) {
        if remainTime <= 0 {
            Terminate(time: t)
        } else {
            SetState(state: .ready)
        }
    }
    func Update(time t: Int) {
        switch state {
            case .ready: waitTime++
            case .running:
                if --remainTime == 0 {
                    Terminate(time: t)
                }
            
            default: break
        }
    }
    
}

class Scheduler {
    
    let maxWaitTime = 10
    var algorithm: Alg
    var list: [Process]? = nil
    var currentTime = 0
    var execIndex: Int? = nil
    var highestPriority = 5
    var highestPriorityIndex: Int? = nil
    
    init (Algorithm alg: Alg) {algorithm = alg}
    
    func SetAlgorithm(Algorithm alg: Alg) {algorithm = alg}
    
    func AddProcess(Name ID: String, remainTime: Int, Priority pr: Int) {
        if list == nil {
            let newProcess = [Process(indentifier: ID, spawnTime: GetTime(), remainTime: remainTime, priority: pr)]
            list = newProcess
        } else {
            let newProcess = Process(indentifier: ID, spawnTime: GetTime(), remainTime: remainTime, priority: pr)
            list!.append(newProcess)
        }
    }
    
    func GetTime() -> Int {return currentTime}
    
    func GetAlgorithm() -> Alg {return algorithm}
    
//  Количество готовых или испольняемых процессов
    func GetProcessNum() -> Int {
        var result = 0
        if list != nil {
            for object in list! {
                if object.GetState() != State.terminated {
                    result++
                }
            }
        } else {
            print("Отсутсвуют процессы в списке!")
        }
        return result
    }
    
//  Поиск процесса с более высоким приоритетом, если такой найден, то возвращается true и изменяются свойства экземпляара класса higestPriority и highestPriorityIndex
    func SearchForPrior() -> Bool {
        if list != nil {
            for (var i=0; i < list!.count; i++) {
                if (list![i].GetPriority() <= highestPriority) && (list![i].GetState() != .terminated) {
                    highestPriority = list![i].GetPriority()
                    highestPriorityIndex = i
                }
            }
        }
        return highestPriorityIndex >= 0
    }
    
    func TurnAround() {
        var i = 0
        if execIndex == nil {
            switch algorithm {
                case .fixedPriority:
                    if SearchForPrior() {
                        execIndex = self.highestPriorityIndex
                        list![execIndex!].SetState(state: State.running)
                    }
                case .FSCF:
                    for (i = 0; list![i].GetState() != State.ready; i++) {}
                    execIndex = i
                    list![execIndex!].SetState(state: State.running)
            }
        }
        currentTime++
        for object in list! {
            object.Update(time: currentTime)
        }
        if GetProcessNum() == 0 {
            return
        }
        i = 0
        var highestPriority = 5
        var highestPriorityIndex: Int? = nil
        for object in list! {
            if object.GetWaitTime() == maxWaitTime {
                if object.GetPriority() < highestPriority {
                    highestPriority = object.GetPriority()
                    highestPriorityIndex = i
                }
                object.waitTime = 0
            }
            i++
        }
        
        if highestPriorityIndex != nil {
            if list![execIndex!].GetState() != .terminated {
            list![execIndex!].SetState(state: .ready)
            }
            execIndex = highestPriorityIndex
            list![execIndex!].SetState(state: .running)
        }
        
        if list![execIndex!].GetState() == .terminated {
            execIndex = nil
            highestPriorityIndex = nil
            highestPriority = 5
        }
    }
}
