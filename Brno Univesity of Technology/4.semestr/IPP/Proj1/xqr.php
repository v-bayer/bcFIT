<?php
#UTF-8
#XQR:xbayer05
#!/usr/bin/php


/**
* predmet:  IPP
* varianta: XML Query
* @author:  Vaclav Bayer, xbayer05@stud.fit.vutbr.cz
*/



/**
* Vypsani chyboveho hlaseni
* @param int
* @return int
*/
function my_error_reporting($error_value, $message){
    switch ($error_value) {
        case '1':
            fprintf(STDERR, "Byl zadan spatny format parametru skriptu, nebo jejich nepovolena kombinace.\n");
            break;

        case '2':
            fprintf(STDERR, "Neexistujici zadany vstupni soubor, nebo chyba otevreni zadaneho vstupniho souboru pro cteni.\n");
            break;

        case '3':
            fprintf(STDERR, "Chyba pri otevirani vystupniho souboru.\n");
            break;

        case '4':
            fprintf(STDERR, "Chybny format vstupniho souboru.\n");
            break;

        case '80':
            fprintf(STDERR, "Chyba pro XQR ulohu. $message\n");
            break;
        
        default:
            fprintf(STDERR, "$message\n");
            break;
    }
    return $error_value;
}


/**
* Parsovani argumentu
* @param int, string
* @return array
*/
function get_options($argc, $argv){

    $options_text = $argv;
    unset($options_text['0']);

    foreach ($options_text as $part_of_options) {

        // --help
        if (preg_match("/--help/", $part_of_options)) {
            $options["help"] = true;
            unset($options_text[array_search($part_of_options, $options_text)]);
        }

        // --input
        if (preg_match("/--input=.+/", $part_of_options)) {
            $options["input"] = trim(substr($part_of_options, 8), "\n");
            unset($options_text[array_search($part_of_options, $options_text)]);
        }

        // --output
        if (preg_match("/--output=.+/", $part_of_options)) {
            $options["output"] = trim(substr($part_of_options, 9), "\n");
            unset($options_text[array_search($part_of_options, $options_text)]);
        }

        // --query
        if (preg_match("/--query=.+/", $part_of_options)) {
            $options["query"] = trim(substr($part_of_options, 8), "\n");
            unset($options_text[array_search($part_of_options, $options_text)]);
        }

        // --qf
        if (preg_match("/--qf=.+/", $part_of_options)) {
            $options["qf"] = trim(substr($part_of_options, 5), "\n");
            unset($options_text[array_search($part_of_options, $options_text)]);
        }

        // -n
        if (preg_match("/^-n$/", $part_of_options)) {
            $options["n"] = true;
            unset($options_text[array_search($part_of_options, $options_text)]);
        }

        // --root
        if (preg_match("/--root=/", $part_of_options)) {
            $options["root"] = trim(substr($part_of_options, 7), "\n");
            unset($options_text[array_search($part_of_options, $options_text)]);
        }
    }

    foreach ($options_text as $part_of_options) {
            // nepovoleny parametr
            exit(my_error_reporting(1, ""));
    }

    if (!isset($options)) {
        exit(my_error_reporting(1, ""));
    }

    return $options;
}



/**
* Vypsani napovedy
*/
function print_help(){
    echo     "NAPOVEDA:\n"
            ."--help                    vytiskne napovedu\n"
            ."--input=filename          zadany vstupni soubor ve formatu XML\n"
            ."--outpu=filename          zadany vystupni soubor ve formatu XML\n"
            ."--query='dotaz'           zadany dotaz v dotazovacim jazce\n"
            ."--qf=filename             dotaz v dotazovacim jazyce v externim textovem souboru\n"
            ."-n                        negenerovat XML hlavicku\n"
            ."--root=element            jmeno parove korenoveho elementu obalujici vysledky\n";
    exit (0);
}


/**
* Osetreni argumentu
* @param array
* @return array
*/
function parse_options($options){

    // --help
    if (isset($options["help"])) {
        if (count($options == 1)) {
            print_help();
            exit(0);
        }else{
            exit(my_error_reporting(1, ""));
        }
    }

    // kolize --query a --qf
    if (isset($options["qf"]) && isset($options["query"]) || !isset($options["qf"]) && !isset($options["query"])) {
        exit(my_error_reporting(1, ""));
    }

    // -n
    if (!isset($options["n"])) {
        $glob["head"] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
    }else{
        $glob["head"] = "";
    }

    // --root
    if (isset($options["root"])) {
        $glob["root"] = $options["root"];
    }

    // --input
    if (isset($options["input"])) {
        if (file_exists($options["input"])) {
            if (($fp = fopen($options["input"], "r")) !== false) {

                $fp_string = fread($fp, filesize($options["input"]));
                $fp_string = utf8_encode($fp_string);
                $glob["xml"] = simplexml_load_string($fp_string);
                fclose($fp);
                if (!$glob["xml"]) exit(my_error_reporting(2,""));
            }else{
                exit(my_error_reporting(2, ""));
            }
        }else{
            exit(my_error_reporting(2, ""));
        }
    }else{
        $glob["xml"] = file_get_contents("php://stdin");
    }

    // --output
    if (isset($options["output"])) {
        $glob["output"] = $options["output"];
    }

    // --qf
    if (isset($options["qf"])){
        if (file_exists($options["qf"])) {
            if (fopen($options["qf"], "r")) {
                $glob["query"] = File_get_contents($options["qf"]);
            }
        }else{
            exit(my_error_reporting(2, ""));
        }
    }

    // --query
    if (isset($options["query"])){
        $glob["query"] = $options["query"];
    }

    return $glob;
}


/**
* Parsovani SQL dotazu, osetreni
* @param array
* @return array
*/
function parse_query($glob){

    $glob["reg_pattern"] = "/SELECT\s+(\S+)\s+(LIMIT\s+(\d+)\s+)?FROM(\s+(\S+))?(\s+WHERE\s+(.*?))?(\s+ORDER\s+BY\s+(\S+)\s+(DESC|ASC))?\s*$/";
    $QR = array();
    $QR_matches = array();

    // jedna se o dotaz?
    if (!preg_match($glob["reg_pattern"], $glob["query"], $QR)) {
        exit(my_error_reporting(80, "Zadany dotaz neni korektni."));
    }

    // SELECT
    $QR["select"] = $QR['1'];


    // LIMIT
    if (!empty($QR['2'])) {
        $QR["limit"] = intval($QR['3']);
        // je to int?
        if (!is_int($QR["limit"])) {
            exit(my_error_reporting(80, "Zadany limit neni cislo typu integer."));
        }
        // je v rozsahu intu?
        if ($QR["limit"] < '0' && $QR["limit"] > PHP_INT_MAX) {
            exit(my_error_reporting(80, "Zadany limit neni v povolenem rozsahu."));
        }

        if ($QR["limit"] === '0'){
            $QR["zero"] = true;
        }
    }

    // FROM
    if (isset($QR['5'])) {
        // ROOT
        if ($QR['5'] === "ROOT") {
            $QR["root"] = true;
        }else{
            // attr
            if ($QR['5']['0'] === ".") {
                $matches = explode('.', $QR['5']);
                $QR["attr"] = $matches['1'];
            }else{
                // element + attr
                if (preg_match("/^(\w+)(\.)+(\w+)/", $QR['5'], $matches)) {
                    $QR["attr"] = $matches['3'];
                    $QR["elem"] = $matches['1'];
                }else{
                    // element
                    $QR["elem"] = $QR['5'];
                }
            }
        }
    }else{
        $QR["empty_file"] = true;
    }

    // WHERE
    if (isset($QR['6'])){
        $QR["where"] = $QR['7'];   
    }

    return $QR;
}


/**
* Parsovani WHERE, osetreni
* @param array
* @return array
*/
function parse_where($QR){

    if (preg_match("/^NOT\s+/", $QR["where"])) {
        $where["NOT_count"] = '0';
        // spocitame NOT
        while ((preg_match("/^NOT\s*/", $QR["where"], $NOT_result)) != false) {
            $where["NOT_count"]++;
            settype($NOT_result['0'], "string");
            $QR["where"] = trim(substr($QR["where"], strlen($NOT_result['0'])), "\n");
        }
    }else{
        $where["NOT_count"] = '0';
    }

    // pocet NOT sude nebo liche?
    if ($where["NOT_count"] % 2 == 0) {
        $where["NOT_count"] = "sude";
    }else{
        $where["NOT_count"] = "liche";
    }
    //echo $where["NOT_count"] . "\n";

    // elem.attr    >|<|=|CONTAINS  literal
    // 1            2               3       <-- parts
    if (preg_match("/^\s*(\.?\w+|\w+\.\w+)\s*(>|<|=|CONTAINS)\s*(\"[\w+|\s*|\W*]*\"|\d+|\"\")$/", $QR["where"], $parts )) {

        // part 1
        // attr
        if ($parts['1']['0'] === ".") {
            $matches = explode('.', $parts['1']);
            $where["attr"] = $matches['1'];
        }else{
            // element + attr
            if (preg_match("/^(\w+)(\.)+(\w+)/", $parts['1'], $matches)) {
                $where["attr"] = $matches['3'];
                $where["elem"] = $matches['1'];
            }else{
                // element
                $where["elem"] = $parts['1'];
            }
        }

        // part 2 & 3
        // CONTAINS
        if ($parts['2'] === "CONTAINS") {
            $where["contains"] = true;
            if (($split_matches = preg_split("/\"/", $parts['3'])) && count($split_matches) > 1) {
                $where["literal"] = $split_matches['1'];
            }else{
                exit(my_error_reporting(80, "Chybna hodnota literalu."));
            }
        }else{
            // operator
            if ($parts["2"] === '<' || $parts["2"] === '>') {
                if (is_numeric($parts['3'])) {
                    $where["literal"] = $parts['3'];
                    //echo "CISLO\n";
                }elseif (($split_matches = preg_split("/\"/", $parts['3'])) && count($split_matches) > 1) {
                    $where["literal"] = $split_matches['1'];
                }else {
                    exit(my_error_reporting(80, "Chybna hodnota literalu."));
                }
            }elseif ($parts["2"] === '=') {
                $where["literal"] = $parts['3'];
            }
            $where["operator"] = $parts["2"];
        }

    }else{
        exit(my_error_reporting(80, "Klausule WHERE neodpovida gramatice."));
    }

    return $where;
}


/**
* Sestaveni cesty pro xpath
* @param array, array
* @return string
*/
function search_action($QR, $where){

    // FROM
    // jestlize FROM neni empty
    if (!isset($QR["empty_file"])) {
        $path = "//*";
            // cteni z korene
            if (isset($QR["root"])) {
                $path = "";
            }else{
                // element + attr
                if (isset($QR["elem"]) && isset($QR["attr"])) {
                    $path .= "//" .$QR["elem"] ."[@" .$QR["attr"] ."][1]";
                // element
                }elseif(isset($QR["elem"]) && !isset($QR["attr"])) {
                    $path .= "//" .$QR["elem"] . "[1]";
                // attr
                }elseif(!isset($QR["elem"]) && isset($QR["attr"])) {
                    $path .= "//*[@" .$QR["attr"] ."][1]";
                }
            }

        // SELECT
        if (!isset($where["contains"]) && !isset($where["operator"])) {
            $path .= "//" .$QR["select"];
        }else{
            if (!empty($where["elem"])) {
                if ($QR["select"] !== $where["elem"]) {
                $path .= "//" .$QR["select"];
                }
            }else{
                $path .= "//" .$QR["select"];
            }
        }

        if (isset($where["NOT_count"])) {
            if ($where["NOT_count"] === "liche") {
                $where["NOT_count"] = "not(";
                $where["NOT_count_end"] = ")";
            }else{
                $where["NOT_count"] = "";
                $where["NOT_count_end"] = "";
            }
        }

        // CONTAINS
        if (isset($where["contains"])) {
            // element + attr
            if (isset($where["elem"]) && isset($where["attr"])) {
                $path .= "//" .$where["elem"] ."[" .$where["NOT_count"] ."contains(@" .$where["attr"] .", " .$where["literal"] .")" .$where["NOT_count_end"] ."]";
            // element
            }elseif(isset($where["elem"]) && !isset($where["attr"])) {
                $path .= "[" .$where["elem"] ."[" .$where["NOT_count"] ."contains(text(), " .$where["literal"] .")" .$where["NOT_count_end"] ."]]";
            // attr
            }elseif(!isset($where["elem"]) && isset($where["attr"])) {
                $path .= "[" .$where["NOT_count"] ."contains(@" .$where["attr"] .", " .$where["literal"] .")" .$where["NOT_count_end"] ."]";
            }
        // OPERATOR
        }elseif(isset($where["operator"])){
            // element + attr
            if (isset($where["elem"]) && isset($where["attr"])) {
                $path .= "//" .$where["elem"] ."[" .$where["NOT_count"] ."@" .$where["attr"] .$where["operator"] .$where["literal"] .$where["NOT_count_end"] ."]";
            // element
            /*}elseif(isset($where["elem"]) && !isset($where["attr"])) {
                $path .= "[" .$where["NOT_count"] .$where["elem"] .$where["operator"] .$where["literal"] .$where["NOT_count_end"] ."]";*/
            }elseif(isset($where["elem"]) && !isset($where["attr"])) {
                $path .= "[" .$where["NOT_count"] .$where["elem"] .$where["operator"] .$where["literal"] .$where["NOT_count_end"] ."]";
            // attr
            }elseif(!isset($where["elem"]) && isset($where["attr"])) {
                $path .= "[" .$where["NOT_count"] ."@" .$where["attr"] .$where["operator"] .$where["literal"] .$where["NOT_count_end"] ."]";
            }
        }
    }else{
        $path = '';
    }

    return $path;
}


/**
* Vytisk do souboru/std out
* @param array, array, string
* @return exit code(int)
*/
function final_print($glob, $QR, $path){

    $i = '0';

    if (isset($glob["root"])) {
        $root = "<" .$glob["root"] .">";
        $root_end = "</" .$glob["root"] .">";
    }else{
        $root = "";
        $root_end = "";
    }

    // prazdny FROM --> prazdny soubor
    if (isset($QR["empty_file"])) {

        if (isset($glob["output"])) {
            $fp = fopen($glob["output"], "w");
            if($fp){
                my_fwrite($fp, $glob["head"]);
                my_fwrite($fp, "$root");
                my_fwrite($fp, "$root_end");
            }else{
                exit($my_error_reporting(3, ""));
            }
        }else{
            file_put_contents("php://stdout", $glob["head"]);
            file_put_contents("php://stdout", $root);
            file_put_contents("php://stdout", $root_end);
        }
        exit(0);
    }

    // provedeni xpath prikazu
    $result = $glob["xml"]->xpath("$path");

    if (isset($glob["output"])) {
        $fp = fopen($glob["output"], "w");
        if($fp){

            if ($result){
                my_fwrite($fp, $glob["head"]);
                my_fwrite($fp, "$root");
                foreach ($result as $key) {
                    if(isset($QR["limit"]) && $QR["limit"] == $i)
                        break;
                    if(isset($QR["zero"]))
                        break;
                    my_fwrite($fp, $key->asXml());
                    $i++;
                }
                my_fwrite($fp, "$root_end");
            }else{
                exit(my_error_reporting(4, ""));
            }
            fclose($fp); 
        }else{
            exit($my_error_reporting(3, ""));
        }
    }else{
        if ($result){
            file_put_contents("php://stdout", $glob["head"]);
            file_put_contents("php://stdout", $root);
            foreach ($result as $key) {
                if(isset($QR["limit"]) && $QR["limit"] == $i)
                        break; 
                if(isset($QR["zero"]))
                        break;
                file_put_contents("php://stdout", $key->asXml());
                $i++;
            }
            file_put_contents("php://stdout", $root_end);
        }else{
            exit(my_error_reporting(4, ""));
        }
    }

    exit(0);
}

function my_fwrite ($fp, $text){
    $text = utf8_encode($text);
    fwrite($fp, $text);
}


/*************************************
* Volani funkci
*/
mb_internal_encoding("UTF-8");

$options = get_options($argc, $argv);
$glob = parse_options($options);
$QR = parse_query($glob);

if (isset($QR["where"])) {
    $where = parse_where($QR);
}else{
    $where = array();
}
if (!isset($QR["empty_file"])) {
    $path = search_action($QR, $where); 
}else{
    $path = "";
}

final_print($glob, $QR, $path)
//*************************************

?>