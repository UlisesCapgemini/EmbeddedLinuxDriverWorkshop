
# En sistemas con múltiples nodos NUMA (como servidores con varios sockets), puedes usar numactl para ejecutar tu binario en un nodo específico:
# Esto fuerza la asignación de memoria y ejecución del proceso en el nodo 0 o 1, respectivamente. Para que esto funcione:
# - El sistema debe tener soporte NUMA (/sys/devices/system/node/ debe tener node0, node1, etc.).
# - El kernel debe tener habilitado CONFIG_NU
numactl --membind=0 --cpubind=0 perf stat ./numa_real_test 0
numactl --membind=1 --cpubind=1 perf stat ./numa_real_test 1
