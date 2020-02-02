# CS564 Assignment 1

A simply shell which implements command 'more' and 'exit' internally and other command will be searched in directory /bin and /usr/bin.

## Author

Group G CC   

Yi Ren (002269013), Wentao Lu (002276355)

YREN19@UBishops.ca, WLU19@UBishops.ca

## Getting Started

### Source code of the project
https://github.com/Grindewald1900/CS564_A1

### Prerequisites

GNU

## Running the tests

Run main.c

Start to input when you see prompt like this：
```
Parent...PID : 934...GCC@sshell>
```
Our program will read the config file(shconfig) to get the values for v and h:
```
Searching configuration from shconfig...
Your configuration: v = 40  h = 75
```
Input  'more' + Space + Filename to show the content of the file(.txt)
```
Parent...PID : 934...GCC@sshell>more r.txt
```
The output should be like this (v lines with h characters per line):

Note the PID here, it's initialed with 1 only to present it's not the child process. The real PID will be output until a child process is executed.
```
Parent...PID : 1...GCC@sshell>more r.txt
Your command:more r.txt

Rap God - Eminem
Look I was gonna go easy on you and
not to hurt your feelings
But I’m only going to get this one chance
...
...
...
v lines totolly
...
...

Parent...PID : 1...
>>Input blank to read more, others to quit!
>>Then press 'Enter' to confirm your input...

```

Input blank to show more and other keys to stop this command.
```
>>Input blank to read more, others to quit!
>>Then press 'Enter' to confirm your input...
a

Parent...PID : 1...>>Command 'more' terminates...
```
Input 'exit' will terminate this program:
```
Parent...PID : 1...GCC@sshell>exit
Your command:exit

Main process terminated normally...
Process finished with exit code 0
```


### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
