with open("out.0", 'r') as f:
    data = f.readlines();

if(len(data) < 2):
    print(data)
    exit()

index = 0
count = 0
for i in range(count * 2):
    if data[i][0] == '#':
        index = i
        count = int(data[i][1:])
        break

data = [d.strip().split(':')[-1] for d in data]
d0 = data[0:index]
d1 = data[index+1:]

for i in range(count):
    if str(i) not in d0:
        print(i, "d0")
    if str(i) not in d1:
        print(i, "d1")


for i in range(1, count):
    if d0[-i] != d1[i-1]:
        print(d0[-1], d1[i-1])
        break
