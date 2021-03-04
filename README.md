## 编译
```
1.cd linux_build
2.cmake ../
3.make
```

## 状态
模块|状态|备注|
--|:--:|:--:|
线程安全消息队列|完成|
服务类型|进行中||
日志服务|||
网络模块部分|||
lua绑定|||

## vscode + wsl + cmake
### launch.json
```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) 启动",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/linux_build/lunet",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "为 gdb 启用整齐打印",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "build"
        }
    ]
}
```
### tasks.json 
```
{
    "tasks": [
        {
            "type": "shell",
            "label": "rebuild",
            "command":"cd linux_build;rm -rf *; cmake .. ; make",
            "group": {
                "kind": "build",
                "isDefault": true
            },
        },
        {
            // 增量编译
            "type": "shell",
            "label": "build",
            "command":"cd linux_build; make",
        }
    ],
    "version": "2.0.0"
}
```

