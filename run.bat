@echo OFF
echo 8
FOR %%x IN (1 2 3 4 5 6) DO FOR %%t IN (1 2 3) DO mpiexec -n 8 D:\Shkola\3rdCourse\Parallel\AffineCipherLab\x64\Release\AffineCipherLabMPI.exe %%x
PAUSE
