/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;
 
/******************* Type utilities ******************************/
 
Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) { 
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
  if(type->typeClass == TP_INT){
    makeIntType();
  }
  else if(type->typeClass == TP_CHAR){
    makeCharType();
  }
  else if(type->typeClass == TP_ARRAY){
    // 
  }
  return type;
}

int compareType(Type* type1, Type* type2) {
  // TODO
  if(type1->typeClass == type2->typeClass) return 1;
    return 0;
}

void freeType(Type* type) {
  // TODO
  if(type->typeClass == TP_ARRAY && type->elementType != NULL){ // Truong hop typeclass == array thi phai free elementType truoc
    freeType(type->elementType);
  }
  free(type);
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue* constVal = (ConstantValue*)malloc(sizeof(ConstantValue));
  constVal->type = TP_INT;
  constVal->intValue = i;
  return constVal;
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue* constVal = (ConstantValue*)malloc(sizeof(ConstantValue));
  constVal->type = TP_CHAR;
  constVal->charValue = ch;
  return constVal;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue* constVal = (ConstantValue*)malloc(sizeof(ConstantValue));
  constVal->type = v->type;
  if(v->type == TP_INT){
    constVal->intValue = v->intValue;
  }
  else if(v->type == TP_CHAR){
    constVal->charValue = v->charValue;
  }
  return constVal;
}

/******************* Object utilities ******************************/

Scope* CreateScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* CreateProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = CreateScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* CreateConstantObject(char *name) {
  // TODO
  Object* constant = (Object*)malloc(sizeof(Object));
  strcpy(constant->name,name);
  constant->kind = OBJ_CONSTANT;
  constant->constAttrs = (ConstantAttributes*)malloc(sizeof(ConstantAttributes));
  constant->constAttrs->value = (ConstantValue*)malloc(sizeof(ConstantAttributes));
  
  return constant;
}

Object* CreateTypeObject(char *name) {
  // TODO
  Object* typ = (Object*)malloc(sizeof(Object));
  strcpy(typ->name,name);
  typ->kind = OBJ_TYPE;
  typ->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
//  typ->typeAttrs->actualType = (TypeAttributes*)malloc(sizeof(TypeAttributes));

  return typ;
}

Object* CreateVariableObject(char *name) {
  // TODO
  Object* var = (Object*)malloc(sizeof(Object));
  strcpy(var->name,name);
  var->kind = OBJ_VARIABLE;
  var->varAttrs = (VariableAttributes*)malloc(sizeof(VariableAttributes));
  var->varAttrs->scope = CreateScope(var,symtab->currentScope);
//  var->varAttrs->type = 

  return var;
}

Object* CreateFunctionObject(char *name) {
  // TODO
  Object* func = (Object*)malloc(sizeof(Object));
  strcpy(func->name,name);
  func->kind = OBJ_FUNCTION;
  func->funcAttrs = (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
  func->funcAttrs->paramList = NULL;    // Cac kieu bien ObjNode luc khoi tao cho bang NULL 
  func->funcAttrs->scope = CreateScope(func,symtab->currentScope);

  return func;
}

Object* CreateProcedureObject(char *name) {
  // TODO
  Object* proc = (Object*)malloc(sizeof(Object));
  strcpy(proc->name,name);
  proc->kind = OBJ_PROCEDURE;
  proc->procAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
  proc->procAttrs->paramList = NULL;
  proc->procAttrs->scope = CreateScope(proc,symtab->currentScope);

  return proc;
}

Object* CreateParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object* param = (Object*)malloc(sizeof(Object));
  strcpy(param->name,name);
  param->kind = OBJ_PARAMETER;
  param->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
  param->paramAttrs->kind = kind;
  param->paramAttrs->type = NULL;
  param->paramAttrs->function = owner;     // Object* owner 

  return param;
}

void freeObject(Object* obj) {
  //TODO
  switch (obj->kind){
  case OBJ_PROGRAM:
    freeScope(obj->progAttrs->scope);
    free(obj->progAttrs);
    free(obj);
    break;
  case OBJ_CONSTANT:
    free(obj->constAttrs->value);
    free(obj->constAttrs);
    free(obj);
    break;
  case OBJ_TYPE:
    free(obj->typeAttrs);
    free(obj);
    break;
  case OBJ_VARIABLE:
    freeScope(obj->varAttrs->scope);
    free(obj->varAttrs);
    free(obj);
    break;
  case OBJ_FUNCTION:
    freeObjectList(obj->funcAttrs->paramList);
    freeScope(obj->funcAttrs->scope);
    free(obj->funcAttrs);
    free(obj);
    break;
  case OBJ_PROCEDURE:
    freeObjectList(obj->procAttrs->paramList);
    freeScope(obj->procAttrs->scope);
    free(obj->procAttrs);
    free(obj);
    break;
  case OBJ_PARAMETER:
    freeType(obj->paramAttrs->type);
    free(obj->paramAttrs->function);  //free Object* owner, khong thuoc cac OBJ_KIND tren ???
    free(obj->paramAttrs);
    free(obj);
    break;
  default:
    break;
  }
}

void freeScope(Scope* scope) {
  // TODO
  freeObjectList(scope->objList);
  free(scope);
}

void freeObjectList(ObjectNode *objList) {
  // TODO
  if(objList->next!=NULL){
    freeObjectList(objList->next);
  }
  freeObject(objList->object);
  free(objList);
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
  // ReferenceList ??
}

void AddObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  ObjectNode* objNode = objList;
  while(objNode!=NULL){
    if(strcmp(objNode->object->name,name)){
      return objNode->object;
    }
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = CreateFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEI");
  param = CreateParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEC");
  param = CreateParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITELN");
  AddObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      AddObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      AddObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  AddObject(&(symtab->currentScope->objList), obj);
}


