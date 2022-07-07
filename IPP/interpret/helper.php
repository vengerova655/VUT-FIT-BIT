<?php
# @author: Veronika Vengerova
# login: xvenge01
# date: 19.4.2022


# singleton class to easier access the configuration of the script chosen by the user
class Tester{
    private $dir;
    private $parser;
    private $interpret;
    private $jexampath;
    private $recursive;
    private $noclean;
    private $intTest;
    private $parseTest;

    private static $instance = null;


    private function __construct()
    {
        # default settings
        $this->dir = "./";
        $this->parser = "./parse.php";
        $this->interpret = "./interpret.py";
        $this->jexampath = "/pub/courses/ipp/jexamxml/";
        $this->recursive = false;
        $this->noclean = false;
        $this->intTest = true;
        $this->parseTest = true;
    }

    # function to make sure there is only 1 instance of this class
    public static function get_instance(){
        if (self::$instance == null){
            self::$instance = new Tester;
        }
        return self::$instance;
    }

    # function to set the path to parser
    private function set_parser($diffParser){
        if(!file_exists($diffParser)){
            print "Parse file does not exist";
            exit(41);
        }
        $this->parser = $diffParser;
    }

    # function to set the path to interpreter
    private function set_interpret($diffInt){
        if(!file_exists($diffInt)){
            print "Interpret file does not exist";
            exit(41);
        }
        $this->interpret = $diffInt;
    }

    # function to set the path to directory of the tests
    private function set_dir($diffDir){
        if (!str_ends_with($diffDir,"/")) {
            $diffDir = $diffDir."/";
        }
        if (! is_dir($diffDir)){
            print $diffDir." is not a directory\n";
            exit(41);
        }
        $this->dir = $diffDir;
    }


    # function to get the path to the directory of the tests
    public function get_dir(){
        return $this->dir;
    }

    # function to set the path to jexamlpath
    private function set_jexampath($jexam){
        if (!str_ends_with($jexam,"/")) {
            $jexam = $jexam."/";
        }
        if (! is_dir($jexam)){
            print $jexam." is not a directory\n";
            exit(41);
        }
        print $jexam;
        $this->jexampath = $jexam;
    }

    # function to set if the user wants to look also in the subdirectories for tests in directory
    private function set_recursive(){
        $this->recursive = true;
    }

    # function to set if the user want to remove temporary files created for testing
    private function set_noclean(){
        $this->noclean = true;
    }

    # function to set if the user want to test only parser
    private function set_p_only(){
        $this->intTest = false;
    }

    # function to set if the user wants to test only interpret
    private function set_i_only(){
        $this->parseTest = false;
    }

    # function to get if the tests should be looked up recursively
    public function recursivity(){
        return $this->recursive;
    }

    # function to get if the temporary files should be kept (if true not clearing if false clearing)
    public function clearance(){
        return $this->noclean;
    }

    # get if parser should be tested
    public  function testing_parse(){
        return $this->parseTest;
    }

    # get if the interpret should be tested
    public function testing_int(){
        return $this->intTest;
    }

    # get the path to the parser
    public function get_parse(){
        return $this->parser;
    }

    # get the path to the xml comparator
    public function get_xml(){
        return $this->jexampath;
    }

    # get the path to the interpreter
    public function get_int(){
        return $this->interpret;
    }
    # function to process arguments to set the script configuration
    public function parse_arg($arguments){

        if(in_array("--help", $arguments)){
            echo "Run script with given parameters:\n";
            echo "--directory=\"path/to/directory/with/tests/\"\n";
            echo "--parse-only  only parser will be tested\n";
            echo "--int-only  only interpret will be tested\n";
            echo "--int-script=\"path/to/interpret/script/\"\n";
            echo "--parse-script=\"path/to/parse/script/\"\n";
            echo "--noclean temporary files created while testing will not be deleted\n";
            echo "--recursive test files will be searched alsoin subdirectories of directory\n";
            echo "--jexampath=\"path/to/jexamxml/directory/\"\n";
            exit(0);
        }

        if(in_array("--noclean", $arguments)){
            $this->set_noclean();
            unset($arguments[array_search("--noclean", $arguments,true)]);
        }

        if(in_array("--recursive", $arguments)){
            $this->set_recursive();
            unset($arguments[array_search("--recursive", $arguments,true)]);
        }

        if(in_array("--parse-only", $arguments)){
            if(!$this->testing_parse()){
                echo "WRONG testing parse\n";
                exit(10);
            }
            $this->set_p_only();
            # remove already processed argument from argument array
            unset($arguments[array_search("--parse-only", $arguments)]);
        }

        if(in_array("--int-only", $arguments)){
            if(!$this->testing_int()){
                echo "WRONG testing int\n";
                exit(10);
            }
            $this->set_i_only();
            # remove already processed argument from argument array
            unset($arguments[array_search("--int-only", $arguments)]);
        }

        foreach ($arguments as $arg) {
            if(str_contains($arg,"=")) {
                list($parameter, $file) = explode("=", $arg, 2);
                if (empty($parameter)) {
                    echo "WRONG parameter, run with --help for legend\n";
                    exit(10);
                }

                switch($parameter){
                    case "--directory":
                        $this->set_dir($file);
                        break;

                    case "--parse-script":
                        if($this->testing_parse()){
                            $this->set_parser($file);
                        }
                        else{
                            echo "ERROR only int is set\n";
                            exit(10);
                        }
                        break;

                    case "--int-script":
                        if($this->testing_int()){
                            $this->set_interpret($file);
                        }
                        else{
                            echo "ERROR only parse is set\n";
                            exit(10);
                        }
                        break;

                    case "--jexampath":
                        if(!$this->testing_parse()){

                            echo "ERROR only int is set\n";
                            exit(10);
                        }
                        $this->set_jexampath($file);
                        break;
                    default:
                        echo "MiSTAKE for help run with --help parameter\n";
                        exit(10);
                }
                # remove already processed argument from argument array
                unset($arguments[array_search($arg, $arguments,true)]);

            }
        }
        if(count($arguments)!=1){
            echo "wrong calling";
            exit(10);
        }
    }
}

# abstract class -> results have the same parameters, functions
# children of this class have different implementations of test_result()
abstract class Result{
    private $path;
    # correct or incorrect classes in styles.css
    private $corr;
    # html ANO NIE output
    private $yesNo;
    # html output
    private $difference;
    # testing result
    private $returnValue;
    #testing expected
    private $expectedValue;
    # path to check xml while parse
    private $xml;

    # function to generate mussing rc file (0)
    public function generate_rc_file($rc){
        file_put_contents($rc, "0");
    }

    # function to create empty file
    public function generate_file($fileName){
        touch($fileName);
    }

    public function __construct($path, $xml)
    {
        # result of the test path
        $this->path = $path;
        # jexamlxml to check xml
        $this->xml = $xml;
    }

    # set return value of the result after testing
    public function set_ret_val($val){
        $this->returnValue = $val;
    }

    # set the representation of the value after testing later in html
    public function set_yesNo($val){
        $this->yesNo = $val;
    }

    # set the output to show after testing later in html
    public function set_diff($val){
        $this->difference = $val;
    }

    # set the representation of the value after testing later in html for choosing correct class in styles css
    public function set_corr($val){
        $this->corr = $val;
    }

    # set the expected value of the test
    public function set_exp_val($val){
        $this->expectedValue = $val;
    }

    # gets the path of the test
    public function get_path(){
        return $this->path;
    }

    # gets the value "correct"/"incorrect" to choose styles.css class
    public function get_corr(){
        return $this->corr;
    }

    # gets the output of the result of the test
    public function get_diff(){
        return $this->difference;
    }

    # gets the representation of the result of the test
    public function get_result_ok(){
        return $this->yesNo;
    }

    # gets the return value of the result of the test
    public function get_ret_val(){
        return $this->returnValue;
    }

    # gets the path to the comparator of xml files
    public function get_xml(){
        return $this->xml;
    }

    # gets the expected value of the result of test
    public function get_expected(){
        return $this->expectedValue;
    }

    #function that is differrent in different subclassess
    abstract public function test_result($file);

    # to style output of wrong result (add breaklines)
    public function incorr_output($output){
        $result ="";
        foreach ($output as $out){
            $result = $result . $out."<br>";
        }
        return $result;
    }
}

# class to specialize parse testing
class ParseResult extends Result{

    public function test_result($parse)
    {
        if (!file_exists($parse)){
            print "Parse file does not exist";
            exit(41);
        }
        $src = $this->get_path().".src";
        if (!file_exists($src)){
            exit(11);
        }
        $in = $this->get_path().".in";
        $rc = $this->get_path().".rc";
        $out = $this->get_path().".out";

        # if files do not exist, generate them
        if (!file_exists($in)){
            $this->generate_file($in);
        }
        if (!file_exists($rc)){
            $this->generate_rc_file($rc);
        }
        if (!file_exists($out)){
            $this->generate_file($out);
        }

        # do the test
        $ExecString = "php8.1 \"" . $parse . "\" <\"" . $src . "\" >\"" . $this->get_path() . ".tmp_out\" 2>\"" . $this->get_path() . ".err\"";
        exec($ExecString, $output, $errOutput);
        $this->set_ret_val($errOutput);
        $this->set_exp_val(file_get_contents($rc));

        # set the values for html styling
        unset($output);
        if ($this->get_ret_val() != $this->get_expected()){
            $this->set_yesNo("NIE");
            $this->set_diff("Ocakavana navratova hodnota: ".$this->get_expected().
                "<br>Ziskana: ".$this->get_ret_val()."<br>");
            $this->set_corr("incorrect");
        }
        # if return values are the same we need to also check output
        else {
            if ($this->get_ret_val() == 0) {
                unset($output);
                # compare using jexamxml
                exec("java -jar \"" . $this->get_xml() . "jexamxml.jar\" \"" . $this->get_path() .
                    ".tmp_out\" \"" . $this->get_path() . ".out\" \"" . $this->get_xml() . "/options\"", $output, $retCode);
                if ($retCode) {
                    $this->set_yesNo("NIE");
                    $this->set_diff($this->incorr_output($output));
                    $this->set_corr("incorrect");
                } else {
                    $this->set_yesNo("ANO");
                    $this->set_diff("Spravne");
                    $this->set_corr("correct");
                }
            }
            # if the number is not zero, we do not need to check the output
            else {
                $this->set_yesNo("ANO");
                $this->set_diff("Spravne");
                $this->set_corr("correct");
            }
        }
    }
}

# class to specialize interpret testing
class InterpretResult extends Result{
    public function test_result($interpret)
    {
        if (!file_exists($interpret)){
            print "Interpret file does not exist";
            exit(41);
        }

        $src = $this->get_path().".src";
        if (!file_exists($src)){
            exit(11);
        }
        $in = $this->get_path().".in";
        $rc = $this->get_path().".rc";
        $out = $this->get_path().".out";

        # generate files that do not exist
        if (!file_exists($in)){
            $this->generate_file($in);
        }
        if (!file_exists($rc)){
            $this->generate_rc_file($rc);
        }
        if (!file_exists($out)){
            $this->generate_file($out);
        }

        # do the test
        $ExecString = "python3.8 \"" . $interpret . "\" --source=\"" . $src."\" <\"" . $in . "\" >\"" .
            $this->get_path()  . ".tmp_out\" 2>\"" . $this->get_path() . ".err\"";
        exec($ExecString, $output, $errOutput);
        $this->set_ret_val($errOutput);
        $this->set_exp_val(file_get_contents($rc));

        # set the values for html styling
        unset($output);
        if ($this->get_ret_val() != $this->get_expected()){
            $this->set_yesNo("NIE");
            $this->set_diff("Ocakavana navratova hodnota: ".$this->get_expected().
                "<br>Ziskana: ".$this->get_ret_val()."<br>");
            $this->set_corr("incorrect");
        }
        else {
            # if return values are the same we need to also check output if the interpretation was correct
            if ($this->get_ret_val() == 0) {
                unset($output);
                exec("diff -u \"" . $this->get_path() . ".out\" \"" . $this->get_path() . ".tmp_out\"", $output, $retCode);

                if ($retCode) {
                    $this->set_yesNo("NIE");
                    $this->set_diff($this->incorr_output($output));
                    $this->set_corr("incorrect");
                } else {

                    $this->set_yesNo("ANO");
                    $this->set_diff("Spravne");
                    $this->set_corr("correct");
                }
            }
            # if the number is not zero, we do not need to check the output
            else {
                $this->set_yesNo("ANO");
                $this->set_diff("Spravne");
                $this->set_corr("correct");
            }
        }
    }
}

# class to specialize to test both parse and interpret
class BothResult extends Result{
    public function test_result($file)
    {
        if(count($file)!=2){
            print "Wrong parameter u shuld pass array with 1. interpreter, 2. parse";
            return;
        }

        $interpret=$file[0];
        $parse= $file[1];
        if (!file_exists($interpret)){
            print "Interpret file does not exist";
            exit(41);
        }
        if (!file_exists($parse)){
            print "Parse file does not exist";
            exit(41);
        }
        $src = $this->get_path().".src";
        if (!file_exists($src)){
            exit(11);
        }

        $in = $this->get_path().".in";
        $rc = $this->get_path().".rc";
        $out = $this->get_path().".out";

        if (!file_exists($in)){
            $this->generate_file($in);
        }
        if (!file_exists($rc)){
            $this->generate_rc_file($rc);
        }
        if (!file_exists($out)){
            $this->generate_file($out);
        }

         $ExecString = "php8.1 \"" . $parse . "\" <\"" . $src . "\" >\"" . $this->get_path() . ".tmp_parse_out\" 2>\""
             . $this->get_path() . ".err_parse\"";
        unset($output);
        exec($ExecString, $output, $errOutput);
        $this->set_ret_val($errOutput);
        $this->set_exp_val(file_get_contents($rc));

        if ($this->get_ret_val() < 30 and $this->get_ret_val() > 0) {
            if($this->get_ret_val() == $this->get_expected()) {
                $this->set_yesNo("ANO");
                $this->set_diff("Spravne");
                $this->set_corr("correct");
            }
            else{
                $this->set_yesNo("NIE");
                $this->set_diff("Ocakavana navratova hodnota: " . $this->get_expected() .
                    "<br>Získaná: " . $this->get_ret_val() . "<br>");
                $this->set_corr("incorrect");
            }
        }
        else{

            $ExecString = "python3.8 \"" . $interpret . "\" --source=\"" . $this->get_path() .".tmp_parse_out\" <\"" .
                $in . "\" >\"" . $this->get_path()  . ".tmp_out\" 2>\"".$this->get_path(). ".err\"";
            exec($ExecString, $output, $errOutput);
            $this->set_ret_val($errOutput);
            $this->set_exp_val(file_get_contents($rc));

            unset($output);
            if ($this->get_ret_val() != $this->get_expected()){
                $this->set_yesNo("NIE");
                $this->set_diff("Ocakavana navratova hodnota: ".$this->get_expected().
                    "<br>Ziskana: ".$this->get_ret_val()."<br>");
                $this->set_corr("incorrect");
            }
            else {
                if ($this->get_ret_val() == 0) {
                    unset($output);
                    exec("diff -u \"" . $this->get_path() . ".out\" \"" . $this->get_path() . ".tmp_out\"",
                        $output, $retCode);

                    if ($retCode) {
                        $this->set_yesNo("NIE");
                        $this->set_diff($this->incorr_output($output));
                        $this->set_corr("incorrect");
                    } else {

                        $this->set_yesNo("ANO");
                        $this->set_diff("Spravne");
                        $this->set_corr("correct");
                    }
                } else {
                    $this->set_yesNo("ANO");
                    $this->set_diff("Spravne");
                    $this->set_corr("correct");
                }
            }
        }
    }
}

class Testing
{
    public $tester;
    public $srcFiles = array();
    public $results;

    function __construct($a)
    {
        $this->tester = $a;
        $this->results = new ResultList();
    }

    public function remove_tmpfiles()
    {
        foreach ($this->srcFiles as $src) {
            $position = strrpos($src, ".src");
            if ($position) {
                $tmpOut = substr_replace($src, ".tmp_out", $position, strlen(".src"));
                $tmpParseOut = substr_replace($src, ".tmp_parse_out", $position, strlen(".src"));
                $tmpErrOut = substr_replace($src, ".err", $position, strlen(".src"));
                $tmpParseErrOut = substr_replace($src, ".err_parse", $position, strlen(".src"));
                if (file_exists($tmpOut)) {
                    unlink($tmpOut);
                }
                if (file_exists($tmpParseOut)) {
                    unlink($tmpParseOut);
                }
                if (file_exists($tmpErrOut)) {
                    unlink($tmpErrOut);
                }
                if (file_exists($tmpParseErrOut)) {
                    unlink($tmpParseErrOut);
                }
            }
        }
    }

    public function check_xml($jaxml){
        if (!is_dir($jaxml)){
            print "does not exist jexaml directory";
            exit(41);
        }
        if (!file_exists($jaxml."jexamxml.jar")){
            print $jaxml."jexamxml.jar does not exist";
            exit(41);
        }
        if (!file_exists($jaxml."options")){
            print $jaxml."options does not exist";
            exit(41);
        }

    }

    public function test()
    {
        $this->load_src_files();
        sort($this->srcFiles);
        foreach ($this->srcFiles as $srcFile) {
            $position = strrpos($srcFile, ".src");
            if ($position) {
                $path = substr_replace($srcFile, "", $position, strlen(".src"));
            }
            $testInt = $this->tester->testing_int();
            $testParse = $this->tester->testing_parse();
            $jaxml = $this->tester->get_xml();
            $this->check_xml($jaxml);
            # check both -> default (output of parse = input of interpret)
            if ($testInt and $testParse) {
                $result = new BothResult($path, $jaxml);
                $fileArray = array();
                array_push($fileArray, $this->tester->get_int(), $this->tester->get_parse());
                $result->test_result($fileArray);
            }
            # test only parse
            elseif (!$testInt and $testParse) {
                $result = new ParseResult($path, $jaxml);
                $result->test_result($this->tester->get_parse());
            }
            # test only interpret
            elseif (!$testParse and $testInt) {
                $result = new InterpretResult($path, $jaxml);
                $result->test_result($this->tester->get_int());
            } else {
                print "Wrong combination, cannot not test either";
                exit(10);
            }
            # adds to the array of results
            $this->results->add_result($result);
        }
        # clean if empty
        if (!$this->tester->clearance()) {
            $this->remove_tmpfiles();
        }
    }

    # add to source files also files in subdirectories
    private function recursive_files($dir)
    {
        if (is_dir($dir)) {
            $files = scandir($dir);
            foreach ($files as $file) {
                if ($file == "." || $file == "..") {
                    continue;
                }
                if (!str_ends_with($dir, "/")) {
                    $fileName = $dir . "/" . $file;
                } else {
                    $fileName = $dir . $file;
                }
                if (is_dir($fileName)) {
                    $this->recursive_files($fileName);
                } else {
                    if (str_ends_with($fileName, ".src")) {
                        array_push($this->srcFiles, $fileName);
                    }
                }
            }
        }
    }

    # returns the number of all src files = tests
    public function all_files()
    {
        return count($this->srcFiles);
    }

    # returns the number of correct tests
    public function count_correct()
    {
        return $this->results->count_correct();
    }

    # load all src files
    public function load_src_files()
    {
        if (!$this->tester->recursivity()) {
            $this->srcFiles = glob($this->tester->get_dir() . "*.src");
        } else {
            $this->recursive_files($this->tester->get_dir());
        }
    }
}

class ResultList {
    public $results = array();

    # adds to the list of results
    public function add_result($result){
        array_push($this->results,$result);
    }

    # counts correct results in resultList
    public function count_correct(){
        $x = 0;
        foreach ($this->results as $result){
            if ($result->get_corr()=="correct"){
               $x = $x +1;
            }
        }
        return $x;
    }
}