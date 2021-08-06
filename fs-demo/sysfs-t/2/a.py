data = ['msg-{}'.format(i)  for i in range(1000)]

for i in data:
        with open("/sys/xx_root/xx", "wb") as f:
                #f.write(i.encode() + b'\0');
                f.write(i.encode());

