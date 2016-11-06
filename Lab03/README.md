# Propagation chaleur sur plaque 2D MPI
### LOG645 - Lab3
## Execution
### Basic
```Unix
mpirun -np NOMBRE_PROC run.sh m n np td h
```

### SSH automation
```Unix
./ssh USER m n np td h nbproc
```

If you want test with same arguments for a bunch of time, setup a configuration by:
```Unix
./config
```

Now you can use this command to launch with config:
```Unix
./sshc
```

### Stats saver
The ssh scripts will save automatically the stats json, in root of project.
To save the stats with timestamp, you need a to have gulp-cli install global on computer.
To launch the saver just do:

```Unix
gulp &
```

Once you finish your work is suggested to kill the gulp task:
```Unix
pkill gulp
```

!!! Warning !!!
This command will kill every instance of gulp process. If other gulp instance are running you'll need to find the specific task with a:
```Unix
ps aux | grep "gulp"
kill -9 {P_ID}
```
