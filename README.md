# OS-pseudo-top

A simple unix command line process monitor that allows you to check cpu usage and memory usage for each process.

## Install on Ubuntu
```
    sudo chmod +x install.sh
    ./install.sh
```

## Documentation

- w -> scroll up the process window  
- s -> scroll down the process window 
- e -> edit mode, when you are in edit mode you can run the following commands
    - PAUSE [PID]-> Send SIGSTOP to [PID] process, stopping its execution
    - RESUME [PID]-> Send SIGCONT to [PID] process, resuming its execution if it is stopped 
    - KILL [PID]-> Send SIGKILL to [PID] process, terminating its execution (hard termination)
    - TERMINATE [PID]-> Send SIGTERM to [PID] process, terminating its execution (soft termination)
- q -> quit
