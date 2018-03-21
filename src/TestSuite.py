import  os

def main():
    compiler = "bin/main"
    if os.name == 'nt':
        compiler = "bin/main.exe"

    if not os.path.isfile(compiler):
        print("Compiler not found! Did you run make?")
        return
        
    testFiles = ["sample/TestSuite/" + f for f in os.listdir("sample/TestSuite/")]
    result_strings = ["Success", "Fail"]
    sucesses = 0
    for file in testFiles:
        print("Testing " + file)
        
        file_should_suceed = file.split('.')[0][-1] == 'S'
        expecting = 0 if file_should_suceed else 1
        print("Expecting: " + result_strings[expecting])
        
        argument_string = compiler + " " + file + " -q"
        result = os.spawnl(os.P_WAIT, compiler, argument_string) # Usually 0 or 1
        if not (result in [0,1]):
            print("ERROR! Compiler crashed during compilation")
            continue
            #break
        print("Result:    " + result_strings[result])
        
        if(result != expecting):
            print("ERROR! check output.txt for information")
            #break
        else:
            sucesses += 1
    print("Completed tests " + str(100*sucesses/len(testFiles)) + "% success rate!")
    

if __name__=="__main__":
    main()