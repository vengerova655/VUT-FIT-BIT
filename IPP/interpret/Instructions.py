# @author: Veronika Vengerova
# login: xvenge01
# date: 19.4.2022

import re
import sys
import Frames as frame
from collections import deque

# ----------------------------------------------------------------------------------------------------------------------
# ---------------------------------------Instruction-Class--------------------------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------

# class representing one instruction with supplementary functions
class Instruction:
    # Instruciton consists of an array of arguments and name of an instruction
    def __init__(self, name):
        # save all instructions in Uppercase letters
        self.name = name.upper()
        self.parameters = []

    # function to add new variable to instruction
    # typ is the type of the variable
    # val is the value of the variable
    def add_var(self,  typ, val):
        value = val
        # typechecking, if given variables have correct values to given types
        if typ == "int":
            try:
                value = int(val)
            except ValueError:
                print("WRONG TYPE, needs to be int", file=sys.stderr)
                exit(32)
        if typ == "float":
            try:
                value = float.fromhex(val)
                value = value.hex()
                # float.fromhex(value)
            except ValueError:
                print("WRONG TYPE, needs to be float", file=sys.stderr)
                exit(32)

        if typ == "bool":
            # in python many values with bool() converter can pass as false/true
            # we accept only true or false strings
            if val.upper() == "TRUE":
                value = "true"
            elif val.upper() == "FALSE":
                value = "false"
            else:
                print("WRONG value, needs to be of type bool", file=sys.stderr)
                exit(32)


        if typ == "string":
            # in strings we need to replace all escape sequences to character representations
            y = re.findall(r"\\[0-9][0-9][0-9]", str(value))
            if y:
                for m in y:
                    x = re.sub(r"\\", "", m)
                    intM = int(x)
                    try:
                        chM = str(chr(intM))
                    except:
                        exit(32)
                    value = value.replace(m, chM)
            value = str(value)
        if val is None:
            value = ""
        if typ == "nil":
            if value != "nil":
                print("WRONG VALUE, needs to be nil", file=sys.stderr)
                exit(53)
            value = "nil"
        self.parameters.append({typ: value})

    # function returns the name of an instruction
    def get_name(self):
        return self.name

    # function returns the number of arguments of this instruction
    def get_parameters_len(self):
        return len(self.parameters)

    # function returns all arguments of this instruction
    def get_parameters(self):
        return self.parameters


# ----------------------------------------------------------------------------------------------------------------------
# ----------------------------------------InstructionList-Class---------------------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------

# aufiliary class for easier control of instructions, all instructions are saved in this class
class InstructionList:
    def __init__(self):
        # instr_list will consist of order of instruction and the instruction itself (class Instruction)
        self.instr_list = dict()
        # variable to control flow of interpret should contains current order of instruction
        # order is meant order given from xml file
        # (represents the line the instruction is written on in file to be interpreted more or less)
        self.curr = 0
        # dictionary to store all labels
        # consists of order of the instruction of label and the label/name of label
        self.labels = {}

    # function to jump to the label with given name label
    # before using label it is needed to check whether label with given name exists
    # to do this we can use function this class's funciton check_label(label)
    def jump(self, label):
        self.curr = self.labels[label]

    # function to check if label with given name/label exists
    # returns true if label with given name exists, false if otherwise
    def check_label(self, label):
        return label in self.labels

    # function returns currently processed instruciton from instruction list
    def get_curr_instr(self):
        return self.instr_list[self.curr]

    # function returns all labels saved in class instance
    def get_labels(self):
        return self.labels

    # funciton returns closest next instruction that should be processed by interpret,
    # or returns -1 if the current instruction is the last one to be processed
    def get_next_instr(self):
        min_val = -1
        if (self.instr_list):
            if (self.curr == max(self.instr_list)):
                # print("WHOOPS uz sme na konci")
                return -1
            try:
                min_val = min(i for i in self.instr_list if i > self.curr)
            except ValueError:
                min_val = -1
        return min_val

    # function to add new instruction to instruction list
    # element in instruction list consists of order of the instruction and instruction(of type Instruction)
    def add_instruction(self, order, instr):
        # instruction with this order already exists
        if order in self.instr_list:
            exit(32)
        # save instruction to instruction list
        self.instr_list[order] = instr

        # if given instruciton is LABEL then we need to add element to dictionary of labels for easier future acess
        if instr.get_name() == 'LABEL':
            # instruction needs to have 1 argument
            if instr.get_parameters_len() != 1:
                exit(52)

            param = instr.get_parameters()
            # label instruction needs to have argument of type label
            if 'label' not in param[0]:
                exit(53)

            # label should not be duplicated
            if self.check_label(param[0]['label']):
                exit(52)

            self.labels[param[0]['label']] = order

    # function returns order of currently processing instruction
    def get_curr(self):
        return self.curr

    # function sets order of currently processing instruction
    # function does not check if given order is valid instruction order
    def set_curr(self, curr):
        self.curr = curr

# TODO maybe remove gete_instruction
    def get_instruction(self, order):
        return 0

    # function returns the whole instruction list
    def get_instructions(self):
        return self.instr_list



# ----------------------------------------------------------------------------------------------------------------------
# ----------------------------------------InstructionController-Class---------------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------

# class to control the instruction
# acts like switch to choose to do different operation with each instruction in instrlist
class InstructionController:
    def __init__(self, instrList, input_file):
        self.FrameStack = frame.Frame_stack()
        # Global frame -> should not be changed
        self.GF = frame.Frame()
        # until created with createframe instruciton both TF and LF are empty
        self.LF = None
        self.TF = None
        self.instructCount = 0
        # stack of data to work with PUSHS POPS
        self.dataStack = deque()
        # current instruction to process
        self.curInstr = None
        # instructions ( instance of InstructionList class )
        self.instrList = instrList
        self.callStack = deque()
        # input file for read instruction
        # represent as stack with each line given as new stack element
        result = deque()
        for line in input_file:
           result.append(line)
        self.input = result

    # function to choose which function to choose from (switch)
    def realize_instruction(self, instr):
        self.curInstr = instr
        method_name = 'instr_' + str(instr.get_name())
        method = getattr(self, method_name, lambda param: exit(32))
        # realize given operation
        method(instr.get_parameters())

    def instr_DEFVAR(self, parameters):
        # instruction needs only 1 parameter of type var
        if len(parameters) != 1:
            exit(32)
        var = parameters[0]
        if 'var' not in var:
            exit(32)
        tmp = var['var']
        # name of variable consists of frame and name of the variable
        fr, name = re.split('@', tmp, 2)
        # define variable in fr frame
        self.define_variable(fr, name)

    # function returns the value
    # symb should consist of directory {[var/int/float/string/nil/bool] => value}
    def get_value(self, symb):
        if 'var' in symb:
            tmpSymb = symb['var']
            frSymb, nameSymb = re.split('@', tmpSymb, 2)
            # gets currently needed frame
            curFrame = self.get_cur_frame(frSymb)
            if curFrame is None:
                exit(55)
            # if variable is defined in frame
            if not curFrame.is_defined(nameSymb):
                exit(54)
            # gets value of variable
            value = curFrame.get_value(nameSymb)
            if value is None:
                print("defined, but not declared variable", file=sys.stderr)
                exit(56)
        # if it is not variable create symbol instances with given values in symb
        elif 'int' in symb:
            value = frame.Symbol(symb['int'], 'int')
        elif 'float' in symb:
            value = frame.Symbol(symb['float'], 'float')
        elif 'string' in symb:
            value = frame.Symbol(symb['string'], 'string')
        elif 'bool' in symb:
            value = frame.Symbol(symb['bool'], 'bool')
        elif 'nil' in symb:
            value = frame.Symbol(symb['nil'], 'nil')
        else:
            exit(53)
        return value

    # function that returns the type of symbol instance
    def get_type_value(self, symb):
        if 'var' in symb:
            tmpSymb = symb['var']
            frSymb, nameSymb = re.split('@', tmpSymb, 2)
            curFrame = self.get_cur_frame(frSymb)
            if curFrame is None:
                exit(55)
            if not curFrame.is_defined(nameSymb):
                exit(54)
            value = curFrame.get_value(nameSymb)
        elif 'int' in symb:
            value = frame.Symbol(symb['int'], 'int')
        elif 'float' in symb:
            value = frame.Symbol(symb['float'], 'float')
        elif 'string' in symb:
            value = frame.Symbol(symb['string'], 'string')
        elif 'bool' in symb:
            value = frame.Symbol(symb['bool'], 'bool')
        elif 'nil' in symb:
            value = frame.Symbol(symb['nil'], 'nil')
        else:
            print("HERE2")
            exit(53)
        return value

    #instruction to move value to variable
    def instr_MOVE(self, parameters):
        # check if correct num of arguments
        if len(parameters) != 2:
            exit(32)
        # variable to move the value to
        var = parameters[0]
        # value to move
        symb = parameters[1]
        if 'var' not in var:
            exit(32)  # TODO check
        tmpVar = var['var']
        frVar, nameVar = re.split('@', tmpVar, 2)
        value = self.get_value(symb)
        self.save_variable(frVar, nameVar, value.get_value(), value.get_type())

    # supplementary function to process 3 parameters with first being the variable name
    # function returns list:
    # [frameName, VariableName, ValueofFirstValue, TypeofFirstValue,ValueofSecondValue, TypeofSecondValue]
    def aritm_3arg(self, parameters):
        var = parameters[0]
        try:
            var = var['var']
        except:
            exit(32)
        fr, name = re.split('@', var, 2)
        symb1 = parameters[1]
        symb2 = parameters[2]
        value1 = self.get_value(symb1)
        value2 = self.get_value(symb2)
        result = [fr, name, value1.get_value(), value1.get_type(), value2.get_value(), value2.get_type()]
        return result

    # ----------------------- ARITHMETIC FUNCTIONS -----------------------------------------------
    # function to process instruction add
    # accepts parameters in list of arguments
    # saves to first element of list addition of the second and third element
    def instr_ADD(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)
        # type checking, both need to be of the same type
        if result[3] != "int" or result[5] != "int":
            if result[3] != "float" or result[5] != "float":
                exit(53)
        try:
            if result[3] == "int":
                tmp = int(int(result[2]) + int(result[4]))
            else:
                val1 = float.fromhex(result[2])
                val2 = float.fromhex(result[4])
                tmp = val1 + val2
                tmp = tmp.hex()
        except ValueError:
            exit(53)
        self.save_variable(result[0], result[1], tmp, result[3])

    # function to process instruction sub
    # accepts parameters in list of arguments
    # saves to first element of list subtraction of the second and third element
    def instr_SUB(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)
        if result[3] != "int" or result[5] != "int":
            if result[3] != "float" or result[5] != "float":
                exit(53)
        try:
            if result[3] == "int":
                tmp = int(int(result[2]) - int(result[4]))
            else:
                val1 = float.fromhex(result[2])
                val2 = float.fromhex(result[4])
                tmp = val1 - val2
                tmp = tmp.hex()
        except ValueError:
            exit(53)
        self.save_variable(result[0], result[1], tmp, result[3])

    # function to process instruction mul
    # accepts parameters in list of arguments
    # saves to first element of list multiplication of the second and third element
    def instr_MUL(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)
        if result[3] != "int" or result[5] != "int":
            if result[3] != "float" or result[5] != "float":
                exit(53)
        try:
            if result[3] == "int":
                tmp = int(int(result[2]) * int(result[4]))
            else:
                val1 = float.fromhex(result[2])
                val2 = float.fromhex(result[4])
                tmp = val1 * val2
                tmp = tmp.hex()
        except ValueError:
            exit(53)
        self.save_variable(result[0], result[1], tmp, result[3])

    # function to process instruction idiv
    # accepts parameters in list of arguments
    # saves to first element of list whole number division of the second and third element
    def instr_IDIV(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)
        if result[3] != "int" or result[5] != "int":
            exit(53)
        try:
            tmp = int(int(result[2]) // int(result[4]))
        except ZeroDivisionError:
            print("ZERO DIVISION", file=sys.stderr)
            exit(57)
        except ValueError:
            exit(53)
        self.save_variable(result[0], result[1], tmp, "int")

    # function to process instruction div (for float)
    # accepts parameters in list of arguments
    # saves to first element of list whole number division of the second and third element
    def instr_DIV(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)
        if result[3] != "float" or result[5] != "float":
            exit(53)
        try:
            tmp1 = float.fromhex(str(result[2]))
            tmp2 = float.fromhex(str(result[4]))
            tmp = (tmp1 / tmp2)
            tmp = tmp.hex()
        except ZeroDivisionError:
            print("ZERO DIVISION", file=sys.stderr)
            exit(57)
        except ValueError:
            exit(53)
        # except:
        #     print("MYSTAKE")
        #     exit(3)
        self.save_variable(result[0], result[1], tmp, "float")

    # function to process instruction lt
    # accepts parameters in list of arguments
    # saves to first element of if the second element is lesser than the third element
    def instr_LT(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)
        if result[3] == "nil" or result[5] == "nil":
            exit(53)

        if not result[3] == result[5]:
            exit(53)

        if result[2] < result[4]:
            self.save_variable(result[0], result[1], "true", "bool")
        else:
            self.save_variable(result[0], result[1], "false", "bool")

    # function to process instruction gt
    # accepts parameters in list of arguments
    # saves to first element of if the second element is greater than the third element
    def instr_GT(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)
        if result[3] == "nil" or result[5] == "nil":
            exit(53)

        if not result[3] == result[5]:
            exit(53)

        if result[2] > result[4]:
            self.save_variable(result[0], result[1], "true", "bool")
        else:
            self.save_variable(result[0], result[1], "false", "bool")

    # function to process instruction eq
    # accepts parameters in list of arguments
    # saves to first element of if the second element is equal to the third element
    def instr_EQ(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)

        if result[3] == result[5] or  result[3] == "nil" or result[5] == "nil":
            if result[2] == result[4]:
                self.save_variable(result[0], result[1], "true", "bool")
            else:
                self.save_variable(result[0], result[1], "false", "bool")
        else:
            exit(53)

    # function to process instruction not
    # accepts parameters in list of arguments
    # saves to first element the negation of the second element
    def instr_NOT(self, parameters):
        if len(parameters) != 2:
            exit(32)
        var = parameters[0]
        try:
            var = var['var']
        except:
            exit(32)

        fr, name = re.split('@', var, 2)
        symb1 = parameters[1]
        value1 = self.get_value(symb1)

        # only bool types
        if value1.get_type() != "bool":
            print("SHOULD BE BOOL WITH AND")
            exit(53)

        value1 = str(value1.get_value())
        if value1.upper() == "FALSE":
            self.save_variable(fr, name, "true", "bool")
        else:
            self.save_variable(fr, name, "false", "bool")

    # supplementary function to get the bool representation of string
    def get_bool(self, value):
        if value == "true":
            value1 = True
        elif value == "false":
            value1 = False
        else:
            exit(53)
        return value1


    # function to process instruction and
    # accepts parameters in list of arguments
    # saves to first element the result of second element and the third element
    def instr_AND(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)

        if result[3] != "bool" or result[5] != "bool":
            exit(53)

        value1 = self.get_bool(result[2])
        value2 = self.get_bool(result[4])

        if (value1 and value2):
            self.save_variable(result[0], result[1], "true", "bool")
        else:
            self.save_variable(result[0], result[1], "false", "bool")

    # function to process instruction or
    # accepts parameters in list of arguments
    # saves to first element the result of second element OR the third element
    def instr_OR(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)

        if result[3] != "bool" or result[5] != "bool":
            exit(53)

        value1 = self.get_bool(result[2])
        value2 = self.get_bool(result[4])

        if (value1 or value2):
            self.save_variable(result[0], result[1], "true", "bool")
        else:
            self.save_variable(result[0], result[1], "false", "bool")


    # function to process instruction int2char
    # accepts parameters as list of arguments
    # saves to first element the character representation of the second value
    def instr_INT2CHAR(self, parameters):
        if len(parameters) != 2:
            exit(32)
        var = parameters[0]
        try:
            var = var['var']
        except:
            exit(32)

        fr, name = re.split('@', var, 2)
        symb1 = parameters[1]
        value1 = self.get_value(symb1)
        #needs to be of type int
        if value1.get_type() != "int":
            print("INT2CHAR mistake", file=sys.stderr)
            exit(53)
        try:
            x = chr(int(value1.get_value()))
        except:
            print("INT2CHAR mistake", file=sys.stderr)
            exit(58)
        self.save_variable(fr, name, x, "string")

    # function to process instruction int2float
    # accepts parameters as list of arguments
    # saves to the first element the value of the second element changed to int
    def instr_INT2FLOAT(self, parameters):
        if len(parameters) != 2:
            exit(32)
        var = parameters[0]
        try:
            var = var['var']
        except:
            exit(32)

        fr, name = re.split('@', var, 2)
        symb1 = parameters[1]
        value1 = self.get_value(symb1)
        # needs to be of type int
        if value1.get_type() != "int":
            print("INT2FLOAT mistake", file=sys.stderr)
            exit(53)
        try:
            result = float(value1.get_value())
        except ValueError:
            print("INT2FLOAT mistake", file=sys.stderr)
            exit(53)

        result = result.hex()
        self.save_variable(fr, name, result, "float")

    # function to process instruction int2float
    # accepts parameters as list of arguments
    # saves to the first element the value of the second element changed to float
    def instr_FLOAT2INT(self, parameters):
        if len(parameters) != 2:
            exit(32)
        var = parameters[0]
        try:
            var = var['var']
        except:
            exit(32)

        fr, name = re.split('@', var, 2)
        symb1 = parameters[1]
        value1 = self.get_value(symb1)
        # needs to be of type int
        if value1.get_type() != "float":
            print("INT2FLOAT mistake", file=sys.stderr)
            exit(53)
        try:
            tmp = float.fromhex(value1.get_value())
            result = int(tmp)
        except ValueError:
            print("INT2FLOAT mistake", file=sys.stderr)
            exit(53)

        self.save_variable(fr, name, str(result), "int")

    # function to process instruction stri2int
    # accepts parameters as list of arguments
    # saves to first element the ordinal value of second elements character
    # on index given by the third element
    def instr_STRI2INT(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)

        if result[3] != "string" or result[5] != "int":
            exit(53)

        try:
            if int(result[4]) < 0:
                exit(58)
        except ValueError:
            exit(53)
        try:
            x = ord(result[2][int(result[4])])
        except:
            # out of range int value
            print("INT2CHAR mistake", file=sys.stderr)
            exit(58)
        self.save_variable(result[0], result[1], x, "int")


    # function to process instruction write
    # accepts parameters as list of arguments
    # writes the value stored in the first element of the list
    def instr_WRITE(self, parameters):
        if len(parameters) != 1:
            print("WRONG")
            exit(32)
        symb = parameters[0]
        x = self.get_value(symb)
        typ = x.get_type()
        val = x.get_value()
        result = str(val)
        if typ == "bool":
            if val == "true":
                result = "true"
            elif val == "false":
                result = "false"
            else:
                exit(53)
        elif typ == "nil":
            if val == "nil":
                # write nil as empty string
                result = ""
            else:
                exit(53)
        elif typ == "float":
            try:
                x = float.fromhex(result)
                result = x.hex()
            except ValueError:
                exit(53)
        else:
            # result = str(val)

            y = re.findall(r"\\[0-9][0-9][0-9]", result)

            # # print(y)
            if y:
                for m in y:
                    m = re.sub(r"\\","",m)
                    # print(m)
                    intM = int(m)
                    try:
                        chM = str(chr(intM))
                    except:
                        exit(53)
                    result = re.sub(r"\\[0-9][0-9][0-9]",chM, result)

        print(result, file = sys.stdout, end="")

    # function to process instruction create framme
    # funciton creates temporary frame
    def instr_CREATEFRAME(self, parameters):
        if len(parameters) != 0:
            exit(32)
        self.TF = frame.Frame()

    # function pushes the current TF frame to LF stack
    def instr_PUSHFRAME(self, parameters):
        if len(parameters) != 0:
            exit(32)
        # the TF was not created before
        if self.TF is None:
            print("TF not created", file=sys.stderr)
            exit(55)
        self.FrameStack.push_Frame(self.TF)
        # current TF was pushed to frame stack, we need to remove from TF
        self.TF = None
        # LF point to the least added frame from the frame stack
        self.LF = self.FrameStack.get_LF()

    # function pops the last added frame from frame stack
    def instr_POPFRAME(self, parameters):
        if len(parameters) != 0:
            exit(32)
        fr = self.FrameStack.get_LF()
        if fr is None:
            print("LF Frame not on stack", file=sys.stderr)
            exit(55)
        # popped frame will be set to temporary TF frame
        self.TF = self.FrameStack.pop_Frame()
        # LF will be set to new last added frame
        self.LF = self.FrameStack.get_LF()

    # function to push value to dataStack (in parameters list the first element)
    def instr_PUSHS(self, parameters):
        if len(parameters) != 1:
            exit(32)
        symb = parameters[0]
        result = self.get_value(symb)
        self.dataStack.append(result)

    # function to saves last pushed value to variable (in parameters list the first element)
    def instr_POPS(self, parameters):
        if len(parameters) != 1:
            exit(32)
        var = parameters[0]
        if 'var' not in var:
            exit(32)
        fr, name = re.split('@', var['var'], 2)

        try:
            result = self.dataStack.pop()
        except:
            print("EMPTY STACK", file=sys.stderr)
            exit(56)
        self.save_variable(fr, name, result.get_value(), result.get_type())

    # function to call to jump to the label (in parameters list the first element)
    # with the possibility to return
    def instr_CALL(self, parameters):
        if len(parameters) != 1:
            exit(32)
        try:
            label = parameters[0]['label']
        except:
            print("CALL to a label, but no label type")
            exit(52)
        nOrder = self.instrList.get_curr()
        if not self.instrList.check_label(label):
            print("label not found")
            exit(52)
        # save the current order of instruction to return, push it to stack
        self.callStack.append(nOrder)
        self.instrList.jump(label)

    # function to jump to the position where the last call was called
    def instr_RETURN(self, parameters):
        if len(parameters) != 0:
            exit(32)
        try:
            x = self.callStack.pop()
        except:
            print("RETURN, but call stack epmty")
            exit(56)
        if x == -1 and len(self.callStack) == 0:
            exit(0)
        else:
            self.instrList.set_curr(x)

    # funciton to process instruction read
    # accepts parameter parameters with list of arguments
    # saves the value of the type given by the second element of list from input to the first element variable
    def instr_READ(self, parameters):
        if len(parameters) != 2:
            exit(32)
        var = parameters[0]
        if 'var' not in var:
            exit(53)
        typ = parameters[1]
        if 'type' not in typ:
            exit(53)
        fr, name = re.split('@', var['var'], 2)
        resultTyp = typ['type']

        # if the stack with from output is empty, save nil value
        if self.input:
            result = self.input.popleft()
            if resultTyp == "int":
                try:
                    result = int(result.strip())
                except ValueError:
                    result = "nil"
                    resultTyp = "nil"
            elif resultTyp == "float":
                try:
                    result = float.fromhex(result.strip())
                    result = result.hex()
                except ValueError:
                    result = "nil"
                    resultTyp = "nil"
            elif typ['type'] == "bool":
                result = result.strip()
                if result.upper() == "TRUE":
                    result = "true"
                else:
                    result = "false"

            elif typ['type'] == "string":
                result = result.strip()
        else:
            result = "nil"
            resultTyp = "nil"
        self.save_variable(fr, name, result, resultTyp)

    # funciton to process instruction concat
    # accepts parameter parameters with list of arguments
    # saves the value of the concatenation of the second element and third element of list
    # to the first element variable
    def instr_CONCAT(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)

        if result[3] != "string" or result[5] != "string":
            exit(53)

        x = result[2] + result[4]
        self.save_variable(result[0], result[1], x, "string")

    # funciton to process instruction strlen
    # accepts parameter parameters with list of arguments
    # saves the length of the second element of the list to the first element variable
    def instr_STRLEN(self, parameters):
        if len(parameters) != 2:
            exit(32)
        var = parameters[0]
        if 'var' not in var:
            exit(53)
        symb1 = parameters[1]
        symb1 = self.get_value(symb1)

        fr, name = re.split('@', var['var'], 2)

        # strlen works only with str type
        if symb1.get_type() != "string":
            exit(53)
        symb1 = symb1.get_value()
        try:
            string = str(symb1)
            result = len(string)
        except:
            exit(58)
        self.save_variable(fr, name, result, "int")

    # funciton to process instruction getchar
    # accepts parameter parameters with list of arguments
    # saves the character of the string second element on the index given by the third element of list
    # to the first element variable
    def instr_GETCHAR(self, parameters):
        if len(parameters) != 3:
            exit(32)
        result = self.aritm_3arg(parameters)

        if result[3] != "string" or result[5] != "int":
            exit(53)
        try:
            if int(result[4]) < 0:
                exit(58)
            x = result[2][result[4]]
        except:
            exit(58)
        self.save_variable(result[0], result[1], x, "string")

    # funciton to process instruction setchar
    # accepts parameter parameters with list of arguments
    # modifies the value of variable given by the first element of list on the index given by the second element
    # to character given by the third element of the list
    def instr_SETCHAR(self, parameters):
        if len(parameters) != 3:
            exit(32)
        var = parameters[0]

        result = self.aritm_3arg(parameters)

        var = self.get_value(var)
        if var.get_type() != "string":
            exit(53)

        if result[3] != "int" or result[5] != "string":
            exit(53)

        try:
            symb2 = result[4][0]
        except:
            exit(58)
        # needs to be of int type as an index
        try:
            i = int(result[2])
        except ValueError:
            exit(58)
        if i >= len(var.get_value()):
            exit(58)
        if i < 0:
            exit(58)
        res = str(var.get_value())[:i] + symb2 +  str(var.get_value())[i+1:]
        self.save_variable(result[0], result[1], res, "string")

    # function to return type of an instance of class Symbol
    def get_type(self, symb):
        if symb is None:
            return ""
        return symb.get_type()

    # function to process instruction type
    # accepts parameter parameters with list of arguments
    # saves the type of the second element of the list
    # to the first element variable
    def instr_TYPE(self, parameters):
        if len(parameters) != 2:
            exit(32)
        var = parameters[0]
        try:
            tmp = var['var']
        except:
            exit(32)
        symb = parameters[1]

        fr, name = re.split('@', tmp, 2)
        result = self.get_type_value(symb)
        # variable is not initialized
        if result is None:
            result = ""
        else:
            result = self.get_type(result)
        self.save_variable(fr, name, result, "string")


    def instr_LABEL(self, parameters):
        if len(parameters) != 1:
            exit(32)
        var = parameters[0]
        if 'label' not in var:
            print("should be label but is not")
            exit(53)

    # funciton to process instruction jump
    # accepts parameter parameters with list of arguments
    # interpreter jumps to the label given by the first element of the list if given label exists
    def instr_JUMP(self, parameters):
        if len(parameters) != 1:
            exit(32)
        var = parameters[0]
        if 'label' not in var:
            print("should be label")
            exit(53)

        label = var['label']

        if not self.instrList.check_label(label):
            print('LABEL NOT DEFINED')
            exit(52)
        self.instrList.jump(label)

    # function to process instruction jumpifeq
    # accepts parameter parameters with list of arguments
    # jumps to the first element of list if the second element is equal to the third element
    def instr_JUMPIFEQ(self, parameters):
        if len(parameters) != 3:
            exit(32)
        var = parameters[0]
        symb1 = self.get_value(parameters[1])
        symb2 = self.get_value(parameters[2])
        if 'label' not in var:
            print("should be label but is not")
            exit(53)
        label = var['label']
        # label to jump to needs to exist
        if not self.instrList.check_label(label):
            exit(52)
        if self.equal_type(symb1, symb2) or symb2.get_type() == "nil" or symb1.get_type() == "nil":
            symb1 = symb1.get_value()
            symb2 = symb2.get_value()
        else:
            exit(53)
        if symb1 == symb2:
            self.instrList.jump(label)

    # function to process instruction jumpifneq
    # accepts parameter parameters with list of arguments
    # jumps to the first element of list if the second element is not equal to the third element
    def instr_JUMPIFNEQ(self, parameters):
        if len(parameters) != 3:
            exit(32)
        var = parameters[0]
        symb1 = self.get_value(parameters[1])
        symb2 = self.get_value(parameters[2])
        if 'label' not in var:
            print("should be label")
            exit(53)

        label = var['label']
        # label needs to exist
        if not self.instrList.check_label(label):
            print('LABEL NOT DEFINED')
            exit(52)

        if self.equal_type(symb1, symb2) or symb2.get_type() == "nil" or symb1.get_type() == "nil":
            symb1 = symb1.get_value()
            symb2 = symb2.get_value()
        else:
            exit(53)
        if symb1 != symb2:
            self.instrList.jump(label)

    # function to process EXIT instruction
    def instr_EXIT(self, parameters):
        if len(parameters) != 1:
            exit(32)
        val = parameters[0]

        value = self.get_value(val)
        if value.get_type() != "int":
            print("Needs to be whole number in exit", file=sys.stderr)
            exit(53)
        try:
            result = int(value.get_value())
        except ValueError:
            print("SHOULD BE WHOLE NUMBER", file=sys.stderr)
            exit(57)


        if result in range(0, 50):
            exit(result)
        else:
            exit(57)

    def instr_DPRINT(self, parameters):
        if len(parameters) != 1:
            exit(53)
        x = parameters[0]
        first_value = list(x.values())[0]
        print(first_value, file=sys.stderr)

    # function for future print of the state of an interpreter
    def int_state(self):
        return 0

    def instr_BREAK(self, parameters):
        if len(parameters) != 0:
            exit(32)
        print(self.int_state(), file=sys.stderr)

    # function returns frame fr
    def get_cur_frame(self, fr):
        if fr == 'GF':
            curFrame = self.GF
        elif fr == 'TF':
            curFrame = self.TF
        elif fr == 'LF':
            curFrame = self.LF
        else:
            # wrong fr name
            exit(55)
        return curFrame

    # function to define variable name in frame fr
    def define_variable(self, fr, name):
        curFrame = self.get_cur_frame(fr)

        # frame does not exist yet
        if curFrame is None:
            exit(55)

        # variable is already defined in frame
        if curFrame.is_defined(name):
            print('already defined\n')
            exit(52)
        curFrame.define_var(name)

    # function to save variable to variable name of frame fr with type typ and value value
    def save_variable(self, fr, name, value, typ):
        curFrame = self.get_cur_frame(fr)

        # frame does not exist yet
        if curFrame is None:
            exit(55)

        # variable is not defined in frame
        if not curFrame.is_defined(name):
            exit(54)

        curFrame.save_variable(name, value, typ)

    # function to check, if 2 instances of class Symbol have the same type
    def equal_type(self,symb1: frame.Symbol, symb2: frame.Symbol):
        return symb1.get_type() == symb2.get_type()
