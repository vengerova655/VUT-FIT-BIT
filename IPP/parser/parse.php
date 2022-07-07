<?php
/**
 * @author xvenge01 (Veronika Vengerová)
 */

ini_set('display_errors', 'stderr');


/////////////////////////
$instructionNum = 1;
$longOpts = array(
    "help",
);
$shortOpts="h";

/**
 * Grammar of IPPcode22 language
 */
$IPP22gram = array(
    "MOVE"=> array("var", "symb"),
    "CREATEFRAME"=> array(),
    "PUSHFRAME"=> array(),
    "POPFRAME"=> array(),
    "DEFVAR"=> array("var"),
    "CALL"=> array("label"),
    "RETURN"=> array(),
    "PUSHS"=> array("symb"),
    "POPS"=> array("var"),
    "ADD"=> array("var", "symb", "symb"),
    "SUB"=> array("var", "symb", "symb"),
    "MUL"=> array("var", "symb", "symb"),
    "IDIV"=> array("var", "symb", "symb"),
    "LT"=> array("var", "symb", "symb"),
    "GT"=> array("var", "symb", "symb"),
    "EQ"=> array("var", "symb", "symb"),
    "AND"=> array("var", "symb", "symb"),
    "OR"=> array("var", "symb", "symb"),
    "NOT"=> array("var", "symb"),
    "INT2CHAR"=> array("var", "symb"),
    "STRI2INT"=> array("var", "symb", "symb"),
    "READ"=> array("var", "type"),
    "WRITE"=> array("symb"),
    "CONCAT"=> array("var", "symb", "symb"),
    "STRLEN"=> array("var", "symb"),
    "GETCHAR"=> array("var", "symb", "symb"),
    "SETCHAR"=> array("var", "symb", "symb"),
    "TYPE"=> array("var", "symb"),
    "LABEL"=> array("label"),
    "JUMP"=> array("label"),
    "JUMPIFEQ"=> array("label", "symb", "symb"),
    "JUMPIFNEQ"=> array("label", "symb", "symb"),
    "EXIT"=> array("symb"),
    "DPRINT"=> array("symb"),
    "BREAK"=> array(),
);

/////////////////////////
/// Kontrola spustenia skriptu so spravnymi parametrami
if($argc == 2){
    if($argv[1] == "--help"){
        echo("Usage: parse.php [--help] <inputFile\n");
        echo("--help            Prints this help.\n");
        exit(0);
    }
    else{
        echo "Wrong usage of parse.php\n";
        exit(0);
    }
}
elseif ($argc>2){
    echo "Wrong usage of parse.php\n";
    exit(0);
}
//////////////////////////


/**
 * Gets input from stdin
 */
$stdin = explode(PHP_EOL, stream_get_contents(STDIN));
if(!$stdin){
    err_exit(11);
}


$instructions = remove_comments($stdin);
$instructions = remove_spaces($instructions);
$result = parse($instructions);
if(!$result){
    err_exit(99);
}

echo $result;
exit(0);


/**
 * Splits instructions into an array of operands and nonterms without spaces
 *
 * @param $instructions (array of instructions to process)
 * @return mixed (array of arrays of operands and nonterms)
 */
function remove_spaces($instructions){
    $regspace = "/^\s*/";
    foreach ($instructions as $key => &$line){
        $line = preg_replace($regspace, '', $line);
        $line = (preg_split('/\h+/', $line));
        foreach ($line as $key1 => &$item){

            if(preg_match("/^\s+/",$item) ||
                $item==''){
                unset($line[$key1]);
                array_values($line);
            }
        }
    }
    unset($line);
    return $instructions;
}

/**
 * Remove comments from input
 *
 * @param $input (Array of instructions with comment)
 * @return array (Array after deleting comments)
 */
function remove_comments($input){

    $reg= "/\s*#.*/";
    $a1 = array( "x" => '');

    foreach ($input as $key => &$line){
        $line = preg_replace($reg, '', $line);
    }
    unset($line);
    $instructions = array_values(array_diff($input,$a1));
    return $instructions;
}

/**
 * Checks header of file, if first line consists of .IPPcode22
 *
 * @param $firstLine (first nonwhitespaced line of stdin in an array of oprands/nonterms)
 * @return bool
 */
function check_IPPcode22($firstLine){
    if ((count($firstLine)!=1)){
        err_exit(21);
    }
    if (strtoupper($firstLine[0]) != ".IPPCODE22"){
        err_exit(21);
    }
    return true;
}

/**
 * Exits with the $ret return value
 *
 * @param $ret (Error number to return)
 * @return void
 */
function err_exit($ret){
    fwrite(STDERR, $ret . PHP_EOL);
    exit($ret);
}

/**
 * Adds new child to xml
 *
 * @param $xml
 * @param $instruction (node of xml to append new child node to)
 * @param $i (order of the new child node)
 * @param $attrVal (attribute value of the child node)
 * @param $name (Value of the child node)
 * @return void
 */
function add_to_xml($xml, $instruction, $i, $attrVal , $name){
    $arg = $xml->createElement("arg" . $i, $name);
    $arg->setAttribute("type", $attrVal);
    $instruction->appendChild($arg);
}

/**
 * Parses array of instructions to xml format
 *
 * @param $instructions (array of instructions (already processed into array of operands/nonterms))
 * @return false|string (string consisting of xml file, or false in case xml could not be saved)
 * @throws DOMException
 */
function parse($instructions)
{
    global $IPP22gram, $instructionNum;

    //regex for label
    $reg_label = "/^[-a-zA-Z_$&%*!?][-a-zA-Z_$&%*!?0-9]*$/";

    $xml = new DOMDocument('1.0','UTF-8');
    $xml->formatOutput=true;

    //go through all instructions/lines from stdin
    foreach ($instructions as $key => $line) {
        $op = strtoupper($line[0]);
        if ($key == "0") {
            check_IPPcode22($line);
            $program = $xml->createElement("program");
            $program->setAttribute("language", "IPPcode22");
            $xml->appendChild($program);
        } else {
            //Instruction not in grammar
            if (!array_key_exists($op, $IPP22gram)) {
                err_exit(22);
            }
            //Wrong number of oprands to this instruciton
            if (count($IPP22gram[strtoupper($line[0])]) != count($line) - 1) {
                err_exit(23);
            }

            $instruction = $xml->createElement("instruction");
            $instruction->setAttribute("order", $instructionNum);
            $instruction->setAttribute("opcode", strtoupper($line[0]));
            $program->appendChild($instruction);
            $instructionNum++;

            //Go through all nonterms of instruction
            for ($i = 0; $i < count($IPP22gram[$op]); $i++) {
                $nonterm = $line[$i + 1];

                //expected variable according to grammar
                if ($IPP22gram[$op][$i] == "var") {
                    $ar = explode('@', $nonterm, 2);
                    if (count($ar) != 2) {
                        err_exit(23);
                    }
                    list($f, $name) = $ar;

                    //If in correct variable format
                    if (preg_match("/^[GLT]F$/", $f)) {
                        if (!preg_match($reg_label, $name)) {
                            err_exit(23);
                        }
                        $changed = htmlspecialchars($name);
                        add_to_xml($xml,$instruction,($i+1), "var",($f . "@" . $changed));
                    } else {
                        err_exit(23);
                    }
                }
                //expected label according to grammar
                elseif ($IPP22gram[$op][$i] == "label") {
                    if (!preg_match($reg_label, $nonterm)) {
                        err_exit(23);
                    }
                    add_to_xml($xml,$instruction,($i+1), "label",$nonterm);
                }
                //expected symb (var/int/string/bool/nil) according to grammar
                elseif ($IPP22gram[$op][$i] == "symb") {
                    $ar = explode('@', $nonterm, 2);
                    //wrong format of symb
                    if (count($ar) != 2) {
                        err_exit(23);
                    }
                    list($f, $name) = $ar;

                    //check if correct symb format
                    if (preg_match("/^[GLT]F$/", $f)) {
                        if (!preg_match($reg_label, $name)) {
                            err_exit(23);
                        }
                        $changed = htmlspecialchars($name);
                        add_to_xml($xml,$instruction,($i+1), "var",($f . "@" . $changed));
                    } else {
                        switch ($f) {
                            case "nil":
                                if ($name != "nil") {
                                    err_exit(23);
                                }
                                add_to_xml($xml,$instruction,($i+1), $f, $name);
                                break;
                            case "string":
                                //regex for special characters -> have to be in format \xyz kde xyz(000-999)
                                $spec_reg = "(\\\\[0-9]{2}([^0-9]|$)|\\\\[0-9]([^0-9]|$)|\\\\[^0-9]|\\\\$)";
                                if (preg_match($spec_reg, $name)) {
                                    err_exit(23);
                                }
                                $changed = htmlspecialchars($name);

                                add_to_xml($xml,$instruction,($i+1), $f, $changed);
                                break;
                            case "int":
                                //regex na int aj vo formate 0x12A456
                                if (!preg_match('/^(([+|-]?[0-9]+)|(0[xX][0-9ABCDEF]+))$/', $name)) {
                                    err_exit(23);
                                }

                                add_to_xml($xml,$instruction,($i+1), $f, $name);
                                break;
                            case "bool":
                                if (!preg_match("/^(true|false)$/", $name)) {
                                    err_exit(23);
                                }

                                add_to_xml($xml,$instruction,($i+1), $f, $name);
                                break;
                            default:
                                err_exit(23);

                        }
                    }
                }
                //expects type according to grammar
                elseif ($IPP22gram[$op][$i] == "type") {

                    if (!preg_match("/^(int|string|bool|)$/", $nonterm)) {
                        err_exit(23);
                    }

                    add_to_xml($xml,$instruction,($i+1), "type", $nonterm);
                }
            }
        }
    }

    $xml_string = $xml->saveXML();
    return $xml_string;
}
///////////////////////
?>