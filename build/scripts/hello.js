// Hello world via the file plugin.

io.stdout.write("Hello, world!\n");

io.stdout.write("JavaScript is nice! We can just print numbers, like " + 2 + "!\n");
io.stdout.write("We can print statements, like 2 + 3 = " + (2+3) + ", and other values like the boolean " + true + "\n");

// What if we want to open a file?

io.stdout.write(Object.keys(io) + "\n");

let that = new io.File("files/example.json");

let bufview = new Int8Array(that.read(that.size));

io.stdout.write("Let's read a file in and then print it out!\n\n");

io.stdout.write(String.fromCharCode.apply(null, bufview));
