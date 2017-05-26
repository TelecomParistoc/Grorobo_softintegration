#! /bin/sh
# /etc/init.d/robotauto

# The following part always gets executed.
echo "This part always gets executed"

# The following part carries out specific functions depending on arguments.
case "$1" in
  start)
    echo "Starting robotMain"
    cd /home/pi/Stardust/prod/main_program
    /home/pi/Stardust/prod/auto_launch/robotMain >> /tmp/robot_output
    echo "robotMain is alive"
    ;;
  stop)
    echo "Stopping robotMain"
    killall robotMain
    echo "robotMain is dead"
    ;;
  *)
    echo "Usage: /etc/init.d/robotauto {start|stop}"
    exit 1
    ;;
esac

exit 0
