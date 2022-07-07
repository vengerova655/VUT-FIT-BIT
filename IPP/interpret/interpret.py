# @author: Veronika Vengerova
# login: xvenge01
# date: 19.4.2022

import sys
import re
from argparse import ArgumentParser
from os import path
import xml.etree.ElementTree as ET
import Instructions as Im
import fileinput

########################################################################################
# Processing arguments that the script runs with
ap = ArgumentParser(add_help=False)
ap.add_argument("--source", type=str)
ap.add_argument("--input", type=str)
ap.add_argument("--help", action='store_true')
# ap.add_argument("--stats", nargs=1, help="INPUT")

args = ap.parse_args()

if args.help:
    print("interpret.py run with parameters:")
    print("--source=\"path/to/source/file\"")
    print("--input=\"path/to/input/file\"")
    print("At least one of the above parameters needs to be set to run script succesfully")
    print("The unset one is set to stdin as default")
    print("Run with --help to see this help")
    if len(sys.argv) > 2:
        print("Parameter help cannot be combined with other parameters", file=sys.stderr)
        exit(10)
    exit(0)

# at least one of either source or input needs to be given
if args.source is None and args.input is None:
    print("at least one of source/input needs to be given", file=sys.stderr)
    exit(10)

# set to get xml_file from stdin as default value
xml_file = sys.stdin

# if source file was given, set our xml_file to given file
if args.source:
    # we need to check if file exists
    if path.isfile(args.source):
        xml_file = open(args.source, 'rt')
    else:
        print("File does not exist", file=sys.stderr)
        exit(11)


# set to get input file from stdin as default value
input_file = sys.stdin

# if input file was given, set our input file to given file
if args.input:
    if path.isfile(args.input):
        input_file = fileinput.input(files=args.input)
    else:
        print("File does not exist", file=sys.stderr)
        exit(11)

# we need to try to parse our xml file
try:
    tree = ET.parse(xml_file)
except:
    print("mistake")
    exit(31)

# get our root from xml file
root = tree.getroot()

# sort all children according to order so we dont have to later work around it
root[:] = sorted(root, key=lambda child: (child.tag, child.get('order')))
# sort all arguments for easier later argument processing
for c in root:
    c[:] = sorted(c, key=lambda child: (child.tag))

# our root from xml file needs to be 'program'
if root.tag != 'program':
    print("Incorrect xml", file=sys.stderr)
    exit(32)

# our root from xml file needs to have attribute 'language' with value 'IPPcode22'
if 'language' in root.attrib:
    if 'IPPcode22' != root.attrib['language']:
        print("Incorrect xml language atrriute", file=sys.stderr)
        exit(32)

# our auxiliary instance of InstructionList to control intrepreting
instrList = Im.InstructionList()

# all instructions (children represent instructions in our xml) need to have given attributes and tags
for child in root:
    if child.tag != 'instruction':
        exit(32)

    if len(child.attrib) != 2:
        print("Incorrect xml atrribute", file=sys.stderr)
        exit(32)

    if 'order' not in child.attrib:
        print("Incorrect xml atrriute", file=sys.stderr)
        exit(32)

    if 'opcode' not in child.attrib:
        print("Incorrect xml atrriute", file=sys.stderr)
        exit(32)
    # order of instruction needs to be number bigger than 1
    order = child.attrib['order']
    uIntMatch = "^[1-9][0-9]*$"
    if not re.match(uIntMatch, order):
        print("mistake, wrong order format", file=sys.stderr)
        exit(32)

    # create new instance of Instruction class to represent instruction
    instr = Im.Instruction(child.attrib['opcode'])

    # supplementary variable to check if the tag of instruction arguments are correct
    i = 1
    for subchild in child:
        # check if instruciton arguments are in the correct format and order
        toMatch = "^arg["+str(i)+"]$"
        if not re.match(toMatch, subchild.tag):
            print("Incorrect xml atrribute argv", file=sys.stderr)
            exit(32)
        # add argument to instruciton
        instr.add_var(subchild.attrib['type'], subchild.text)
        i = i+1
    # add whole instruction to the instruction list class
    instrList.add_instruction(int(order), instr)

# instance of a class to control interpreting
switch = Im.InstructionController(instrList, input_file)

# main loop to process instructions until the end of program
while(1):
    if instrList.get_curr() == -1:
        break
    curOrder = instrList.get_next_instr()
    if curOrder == -1:
        break
    instrList.set_curr(curOrder)
    curInst = instrList.get_curr_instr()
    switch.realize_instruction(curInst)


