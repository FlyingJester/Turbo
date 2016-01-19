// Hello world via the file plugin.

file.stdout.write("Hello, world!\n");

file.stdout.write("JavaScript is nice! We can just print numbers, like " + 2 + "!\n");
file.stdout.write("We can print statements, like 2 + 3 = " + (2+3) + ", and other values like the boolean " + true + "\n");

// What if we want to open a file?

file.stdout.write(Object.keys(file) + "\n");

let that = new file.File("files/example.json");

file.stdout.write(that + "");

file.stdout.write(Object.keys(file.File) + "\n");

let bufview = new Int8Array(that.read(that.size));

file.stdout.write("Let's read a file in and then print it out!\n\n");

file.stdout.write(String.fromCharCode.apply(null, bufview));
