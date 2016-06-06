/*
 * Copyright 2014 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// independent from idl_parser, since this code is not needed for most clients

#include <string>

#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/idl.h"
#include "flatbuffers/util.h"
#include "flatbuffers/code_generators.h"

namespace flatbuffers {
    namespace objc {
        
        class ObjcGenerator : public BaseGenerator {
            
            
            
            struct CommentConfig {
                const char *first_line;
                const char *content_line_prefix;
                const char *last_line;
            };
            
            struct LanguageParameters {
                // Whether function names in the language typically start with uppercase.
                bool first_camel_upper;
                const char *header_file_extension;
                const char *content_file_extension;
                const char *string_type;
                const char *bool_type;
                const char *open_curly;
                const char *const_decl;
                const char *unsubclassable_decl;
                const char *enum_decl;
                const char *enum_separator;
                const char *getter_prefix;
                const char *getter_suffix;
                const char *inheritance_marker;
                const char *namespace_ident;
                const char *namespace_begin;
                const char *namespace_end;
                const char *set_bb_byteorder;
                const char *get_bb_position;
                const char *get_fbb_offset;
                const char *includes;
                CommentConfig comment_config;
            };
            
            LanguageParameters language_parameters = {
                false,
                ".h",
                ".m",
                "NSString",
                "BOOL ",
                " {\n",
                " final ",
                "final ",
                "typedef NS_ENUM(",
                ",\n",
                "()",
                "",
                " : ",
                "package ",
                ";",
                "",
                "_bb.order(ByteOrder.LITTLE_ENDIAN); ",
                "position()",
                "offset()",
                ""
                "",
                {
                    "/**",
                    " *",
                    " */",
                },
            };
            
            
        public:
            
            static std::string nameSpace(const Parser &parser) {
                
                std::string namespace_general;
        
                auto &namespaces = parser.namespaces_.back()->components;
                
                for (auto it = namespaces.begin(); it != namespaces.end(); ++it) {

                    namespace_general += *it;
                    
                }
                
                return namespace_general;
            }
            
            static bool SaveHeader(const LanguageParameters &lang, const Parser &parser,
                                  const std::string &defname, const std::string &classcode,
                                  const std::string &path, bool needs_includes, bool onefile) {
                if (!classcode.length()) return true;
                
                std::string namespace_dir = path;  // Either empty or ends in separator.
                auto &namespaces = parser.namespaces_.back()->components;
                for (auto it = namespaces.begin(); it != namespaces.end(); ++it) {
                    if (!onefile) {
                        namespace_dir += *it + kPathSeparator;
                        break;
                    }
                    
                }

                EnsureDirExists(namespace_dir);
                
                std::string code = "// automatically generated, do not modify !!!\n\n";
                if (needs_includes) code += lang.includes;
                code += classcode;
                auto filename = namespace_dir + defname + lang.header_file_extension;
                return SaveFile(filename.c_str(), code, false);
            }
            
            static bool SaveClass(const LanguageParameters &lang, const Parser &parser,
                                   const std::string &defname, const std::string &classcode,
                                   const std::string &path, bool needs_includes, bool onefile) {
                if (!classcode.length()) return true;
                
                std::string namespace_dir = path;  // Either empty or ends in separator.
                auto &namespaces = parser.namespaces_.back()->components;
                for (auto it = namespaces.begin(); it != namespaces.end(); ++it) {
                    if (!onefile) {
                        namespace_dir += *it + kPathSeparator;
                        break;
                    }
                    
                }
                EnsureDirExists(namespace_dir);
                
                std::string code = "// automatically generated, do not modify !!!\n\n";
                if (needs_includes) code += lang.includes;
                code += classcode;
                auto filename = namespace_dir + defname + lang.content_file_extension;
                return SaveFile(filename.c_str(), code, false);
            }
            
            // Generate a documentation comment, if available.
            static void GenComment(const std::vector<std::string> &dc, std::string *code_ptr,
                            const CommentConfig *config, const char *prefix = NULL) {
                if (dc.begin() == dc.end()) {
                    // Don't output empty comment blocks with 0 lines of comment content.
                    return;
                }
                
                std::string &code = *code_ptr;
                if (config != nullptr && config->first_line != nullptr) {
                    code += std::string(prefix) + std::string(config->first_line) + "\n";
                }
                std::string line_prefix = std::string(prefix) +
                ((config != nullptr && config->content_line_prefix != nullptr) ?
                 config->content_line_prefix : "///");
                for (auto it = dc.begin();
                     it != dc.end();
                     ++it) {
                    code += line_prefix + *it + "\n";
                }
                if (config != nullptr && config->last_line != nullptr) {
                    code += std::string(prefix) + std::string(config->last_line) + "\n";
                }
            }
            
            static std::string GenNumberMethod(const BaseType &type) {
                
                std::string type_str;
                switch (type) {
                    case BASE_TYPE_UTYPE:
                        type_str = "numberWithChar";
                        break;
                    case BASE_TYPE_BOOL:
                        type_str = "numberWithBool";
                        break;
                    case BASE_TYPE_UCHAR:
                        type_str = "numberWithUnsignedChar";
                        break;
                    case BASE_TYPE_CHAR:
                        type_str = "numberWithChar";
                        break;
                    case BASE_TYPE_SHORT:
                        type_str = "numberWithShort";
                        break;
                    case BASE_TYPE_USHORT:
                        type_str = "numberWithUnsignedShort";
                        break;
                    case BASE_TYPE_INT:
                        type_str = "numberWithInt";
                        break;
                    case BASE_TYPE_UINT:
                        type_str = "numberWithUnsignedInt";
                        break;
                    case BASE_TYPE_LONG:
                        type_str = "numberWithLongLong";
                        break;
                    case BASE_TYPE_ULONG:
                        type_str = "numberWithUnsignedLongLong";
                        break;
                    case BASE_TYPE_FLOAT:
                        type_str = "numberWithFloat";
                        break;
                    case BASE_TYPE_DOUBLE:
                        type_str = "numberWithDouble";
                        break;
                    default:
                        break;
                }
                
                return type_str;
            }
            
            static std::string GenNumberType(const BaseType &type) {
                
                std::string type_str;
                switch (type) {
                    case BASE_TYPE_UTYPE:
                        type_str = "FBNumberInt8";
                        break;
                    case BASE_TYPE_BOOL:
                        type_str = "FBNumberBool";
                        break;
                    case BASE_TYPE_UCHAR:
                        type_str = "FBNumberUint8";
                        break;
                    case BASE_TYPE_CHAR:
                        type_str = "FBNumberInt8";
                        break;
                    case BASE_TYPE_SHORT:
                        type_str = "FBNumberInt16";
                        break;
                    case BASE_TYPE_USHORT:
                        type_str = "FBNumberUint16";
                        break;
                    case BASE_TYPE_INT:
                        type_str = "FBNumberInt32";
                        break;
                    case BASE_TYPE_UINT:
                        type_str = "FBNumberUint32";
                        break;
                    case BASE_TYPE_LONG:
                        type_str = "FBNumberInt64";
                        break;
                    case BASE_TYPE_ULONG:
                        type_str = "FBNumberUint64";
                        break;
                    case BASE_TYPE_FLOAT:
                        type_str = "FBNumberFloat";
                        break;
                    case BASE_TYPE_DOUBLE:
                        type_str = "FBNumberDouble";
                        break;
                    case BASE_TYPE_UNION:
                        type_str = "FBNumberInt64";
                        break;
                        
                    default:
                        break;
                }
                
                return type_str;
            }
            
            static std::string GenGetMethod(const BaseType &type) {
                
                std::string type_str;
                switch (type) {
                    case BASE_TYPE_UTYPE:
                        type_str = "Int8";
                        break;
                    case BASE_TYPE_BOOL:
                        type_str = "Bool";
                        break;
                    case BASE_TYPE_UCHAR:
                        type_str = "Uint8";
                        break;
                    case BASE_TYPE_CHAR:
                        type_str = "Int8";
                        break;
                    case BASE_TYPE_SHORT:
                        type_str = "Int16";
                        break;
                    case BASE_TYPE_USHORT:
                        type_str = "Uint16";
                        break;
                    case BASE_TYPE_INT:
                        type_str = "Int32";
                        break;
                    case BASE_TYPE_UINT:
                        type_str = "Uint32";
                        break;
                    case BASE_TYPE_LONG:
                        type_str = "Int64";
                        break;
                    case BASE_TYPE_ULONG:
                        type_str = "Uint64";
                        break;
                    case BASE_TYPE_FLOAT:
                        type_str = "Float";
                        break;
                    case BASE_TYPE_DOUBLE:
                        type_str = "Double";
                        break;
                    case BASE_TYPE_UNION:
                        type_str = "int64_t";
                        break;
                        
                    default:
                        break;
                }
                
                return type_str;
            }
            
            static std::string GenTypeBasic(const BaseType &type) {
                
                std::string type_str;
                switch (type) {
                    case BASE_TYPE_BOOL:
                        type_str = "BOOL";
                        break;
                    case BASE_TYPE_UCHAR:
                        type_str = "uint8_t";
                        break;
                    case BASE_TYPE_CHAR:
                        type_str = "int8_t";
                        break;
                    case BASE_TYPE_SHORT:
                        type_str = "int16_t";
                        break;
                    case BASE_TYPE_USHORT:
                        type_str = "uint16_t";
                        break;
                    case BASE_TYPE_INT:
                        type_str = "int32_t";
                        break;
                    case BASE_TYPE_UINT:
                        type_str = "uint32_t";
                        break;
                    case BASE_TYPE_LONG:
                        type_str = "int64_t";
                        break;
                    case BASE_TYPE_ULONG:
                        type_str = "uint64_t";
                        break;
                    case BASE_TYPE_FLOAT:
                        type_str = "float";
                        break;
                    case BASE_TYPE_DOUBLE:
                        type_str = "double";
                        break;
                    case BASE_TYPE_UNION:
                        type_str = "int64_t";
                        break;
                        
                    default:
                        break;
                }
                
                return type_str;
            }

            
            static std::string GenTypePointer(const Parser &parser, const Type &type) {
                switch (type.base_type) {
                    case BASE_TYPE_STRING:
                        return "NSString";
                    case BASE_TYPE_VECTOR:
                        return "FBMutableArray";
                    case BASE_TYPE_STRUCT:
                        return nameSpace(parser) + type.struct_def->name;
                    case BASE_TYPE_UNION:
                        // fall through
                    default:
                        return "FBTable";
                }
            }
            
            static std::string GenTypeGet(const Parser &parser, const Type &type) {
                return IsScalar(type.base_type)
                ? GenTypeBasic(type.base_type)
                : GenTypePointer(parser, type);
            }
            
            // Find the destination type the user wants to receive the value in (e.g.
            // one size higher signed types for unsigned serialized values in Java).
            static Type DestinationType(const LanguageParameters &lang, const Parser &parser, const Type &type,
                                        bool vectorelem) {
            
                switch (type.base_type) {
                        // We use int for both uchar/ushort, since that generally means less casting
                        // than using short for uchar.
                    case BASE_TYPE_UCHAR:  return Type(BASE_TYPE_INT);
                    case BASE_TYPE_USHORT: return Type(BASE_TYPE_INT);
                    case BASE_TYPE_UINT:   return Type(BASE_TYPE_LONG);
                    case BASE_TYPE_VECTOR:
                        if (vectorelem)
                            return DestinationType(lang, parser, type.VectorType(), vectorelem);
                        // else fall thru:
                    default: return type;
                }
            }
            
            // Generate destination type name
            static std::string GenTypeNameDest(const LanguageParameters &lang, const Parser &parser, const Type &type)
            {
                return GenTypeGet(parser, DestinationType(lang, parser, type, true));
            }
            
            // Mask to turn serialized value into destination type value.
            static std::string DestinationMask(const LanguageParameters &lang,
                                               const Type &type, bool vectorelem) {
                
                switch (type.base_type) {
                    case BASE_TYPE_UCHAR:  return " & 0xFF";
                    case BASE_TYPE_USHORT: return " & 0xFFFF";
                    case BASE_TYPE_UINT:   return " & 0xFFFFFFFFL";
                    case BASE_TYPE_VECTOR:
                        if (vectorelem)
                            return DestinationMask(lang, type.VectorType(), vectorelem);
                        // else fall thru:
                    default: return "";
                }
            }
            
            // Casts necessary to correctly read serialized data
            static std::string DestinationCast(const LanguageParameters &lang, const Parser &parser,
                                               const Type &type) {
                if (type.base_type == BASE_TYPE_VECTOR) {
                    return DestinationCast(lang, parser, type.VectorType());
                } else {
                    
                    // Cast necessary to correctly read serialized unsigned values.
//                    if (type.base_type == BASE_TYPE_UINT) return "(long)";
                
                }
                return "";
            }
            
            // Cast statements for mutator method parameters.
            // In Java, parameters representing unsigned numbers need to be cast down to their respective type.
            // For example, a long holding an unsigned int value would be cast down to int before being put onto the buffer.
            // In C#, one cast directly cast an Enum to its underlying type, which is essential before putting it onto the buffer.
            static std::string SourceCast(const LanguageParameters &lang, const Parser &parser,
                                          const Type &type,
                                          bool castFromDest) {
                if (type.base_type == BASE_TYPE_VECTOR) {
                    return SourceCast(lang, parser, type.VectorType(), castFromDest);
                } else {
                    
                    if (castFromDest) {
                        if (type.base_type == BASE_TYPE_UINT) return "(int)";
                        else if (type.base_type == BASE_TYPE_USHORT) return "(short)";
                        else if (type.base_type == BASE_TYPE_UCHAR) return "(byte)";
                    }
                }
                return "";
            }
            
            static std::string SourceCast(const LanguageParameters &lang, const Parser &parser,
                                          const Type &type) {
                return SourceCast(lang, parser, type, true);
            }
            
            static std::string SourceCastBasic(const LanguageParameters &lang, const Parser &parser,
                                               const Type &type,
                                               bool castFromDest) {
                return IsScalar(type.base_type) ? SourceCast(lang, parser, type, castFromDest) : "";
            }
            
            static std::string SourceCastBasic(const LanguageParameters &lang, const Parser &parser,
                                               const Type &type) {
                return SourceCastBasic(lang, parser, type, true);
            }
            
            
            void GenEnum(const LanguageParameters &lang, const Parser &parser, EnumDef &enum_def,
                                std::string *code_ptr) {
                std::string &code = *code_ptr;
                if (enum_def.generated) return;
                
                // Generate enum definitions of the form:
                // public static (final) int name = value;
                // In Java, we use ints rather than the Enum feature, because we want them
                // to map directly to how they're used in C/C++ and file formats.
                // That, and Java Enums are expensive, and not universally liked.
                
                GenComment(enum_def.doc_comment, code_ptr, &lang.comment_config);
                code += lang.enum_decl + GenTypeBasic(enum_def.underlying_type.base_type)+nameSpace(parser) + ", " + enum_def.name + ") ";
                code += lang.open_curly;

                for (auto it = enum_def.vals.vec.begin();
                     it != enum_def.vals.vec.end();
                     ++it) {
                    auto &ev = **it;
                    GenComment(ev.doc_comment, code_ptr, &lang.comment_config, "  ");
                    code += " " + nameSpace(parser) + ev.name + " = ";
                    code += NumToString(ev.value);
                    code += lang.enum_separator;
                }
                
                // Close the class
                code += "};\n\n";
            }
            static void GenClass(const LanguageParameters &lang, const Parser &parser,
                                  StructDef &struct_def, std::string *code_ptr, std::string *content_code_ptr) {
                if (struct_def.generated) return;
                std::string tempcode;
                std::string headercode;
                std::string &code = tempcode;
                std::string &content_code = *content_code_ptr;
                std::string &real_code = *code_ptr;
                
                
                // Generate a struct accessor class, with methods of the form:
                // public type name() { return bb.getType(i + offset); }
                // or for tables of the form:
                // public type name() {
                //   int o = __offset(offset); return o != 0 ? bb.getType(o + i) : default;
                // }
                GenComment(struct_def.doc_comment, code_ptr, &lang.comment_config);
                headercode += "#import \"FBTable.h\"\n";
                code += "@interface " + nameSpace(parser) + struct_def.name + lang.inheritance_marker;
                code += struct_def.fixed ? "FBTable" : "FBTable";
                code += " \n\n";
                
                GenComment(struct_def.doc_comment, content_code_ptr, &lang.comment_config);
                content_code += "#import \""+nameSpace(parser) + struct_def.name+".h\"\n\n";
                content_code += "@implementation " + nameSpace(parser) + struct_def.name;
                content_code += " \n\n";
                size_t bytesize = 0;
                
                for (auto it = struct_def.fields.vec.begin();
                     it != struct_def.fields.vec.end();
                     ++it) {
                    auto &field = **it;
                    if (field.deprecated) continue;
                    GenComment(field.doc_comment, code_ptr, &lang.comment_config, "  ");
                    std::string type_name = GenTypeGet(parser, field.value.type);
//                    std::string type_name_dest = GenTypeNameDest(lang, parser, field.value.type);
                    std::string type_name_dest = GenTypeGet(parser, field.value.type);
                    std::string dest_mask = DestinationMask(lang, field.value.type, true);
                    std::string dest_cast = DestinationCast(lang, parser, field.value.type);
                    std::string src_cast = SourceCast(lang, parser, field.value.type);
                    std::string method_start = "- (" + type_name_dest + ") " +
                    MakeCamel(field.name, lang.first_camel_upper);
                    
                    // Most field accessors need to retrieve and test the field offset first,
                    // this is the prefix code for that:
                    auto offset_prefix = " { int o = fb_offset(" +
                    NumToString(field.value.offset) +
                    "); return o != 0 ? ";
                    
                    switch (field.value.type.base_type) {
                        case BASE_TYPE_STRING:{
                            
                            code += "@property (nonatomic, strong)"+type_name_dest+" *"+field.name+";\n\n";
                            content_code += "- (" + type_name_dest + " *) " + field.name + " {\n\n";
                            content_code += "    _"+field.name+" = [self fb_getString:"+NumToString(field.value.offset)+" origin:_"+field.name+"];\n\n";
                            content_code += "    return _"+field.name+";\n\n";
                            content_code += "}\n\n";
                            
                            
                            content_code += "- (void) add_"+field.name+" {\n\n";
                            content_code += "    [self fb_addString:_"+field.name+" voffset:"+NumToString(field.value.offset)+" offset:"+NumToString(bytesize+4)+"];\n\n";
                            content_code += "    return ;\n\n";
                            content_code += "}\n\n";
                            
                        }
                            break;
                        case BASE_TYPE_STRUCT:{
                            headercode += "#import \""+nameSpace(parser) + field.value.type.struct_def->name+".h\"\n";
                            
                            code += "@property (nonatomic, strong)"+type_name_dest+" *"+field.name+";\n\n";
                            content_code += "- (" + type_name_dest + " *) " + field.name + " {\n\n";
                            if (struct_def.fixed || field.value.type.struct_def->fixed) {
                                content_code += "    _"+field.name+" = [self fb_getStruct:"+NumToString(field.value.offset)+" origin:_"+field.name+" className:["+nameSpace(parser) + field.value.type.struct_def->name+" class]];\n\n";
                            }else{
                                content_code += "    _"+field.name+" = [self fb_getTable:"+NumToString(field.value.offset)+" origin:_"+field.name+" className:["+nameSpace(parser) + field.value.type.struct_def->name+" class]];\n\n";
                            }
                            content_code += "    return _"+field.name+";\n\n";
                            content_code += "}\n\n";
                            
                            
                            
                            content_code += "- (void) add_"+field.name+" {\n\n";
                            if (struct_def.fixed || field.value.type.struct_def->fixed) {
                                content_code += "    [self fb_addStruct:_"+field.name+" voffset:"+NumToString(field.value.offset)+" offset:"+NumToString(bytesize+4)+"];\n\n";
                            }else{
                                content_code += "    [self fb_addTable:_"+field.name+" voffset:"+NumToString(field.value.offset)+" offset:"+NumToString(bytesize+4)+"];\n\n";
                            }
                            content_code += "    return ;\n\n";
                            content_code += "}\n\n";
                        }
                            break;
                        case BASE_TYPE_VECTOR:{
                            
//                            auto underlying_type = field.value.type.base_type == BASE_TYPE_VECTOR
//                            ? field.value.type.VectorType()
//                            : field.value.type;
                            
                            auto underlying_type = field.value.type.VectorType();
                            
                            switch (field.value.type.element) {
                                case BASE_TYPE_STRUCT:{
                                    
                                    headercode += "#import \""+nameSpace(parser) + field.value.type.struct_def->name+".h\"\n";
                                    
                                    std::string element_name = nameSpace(parser) + field.value.type.struct_def->name;
                                    std::string element_type_full = "<"+element_name+" *>";
                                    code += "@property (nonatomic, strong)"+type_name_dest+element_type_full+" *"+field.name+";\n\n";
                                    content_code += "- (" + type_name_dest + element_type_full + " *) " + field.name + " {\n\n";
                                    if (field.value.type.struct_def->fixed) {
                                        
                                        content_code += "    _"+field.name+" = [self fb_getStructs:"+NumToString(field.value.offset)+" origin:_"+field.name+" className:["+nameSpace(parser) + field.value.type.struct_def->name+" class] byteSize:"+NumToString(InlineSize(underlying_type))+"];\n\n";
                                        
                                        
                                    }else{
                                        
                                        content_code += "    _"+field.name+" = [self fb_getTables:"+NumToString(field.value.offset)+" origin:_"+field.name+" className:["+nameSpace(parser) + field.value.type.struct_def->name+" class]];\n\n";
                                        
                                    }
                                    content_code += "    return _"+field.name+";\n\n";
                                    content_code += "}\n\n";
                                    
                                    
                                    
                                    content_code += "- (void) add_" + field.name + " {\n\n";
                                    if (field.value.type.struct_def->fixed) {
                                        content_code += "    [self fb_addStructs:_"+field.name+" voffset:"+NumToString(field.value.offset)+" offset:"+NumToString(bytesize+4)+"];\n\n";
                                    }else{
                                        content_code += "    [self fb_addTables:_"+field.name+" voffset:"+NumToString(field.value.offset)+" offset:"+NumToString(bytesize+4)+"];\n\n";
                                    }
                                    content_code += "    return ;\n\n";
                                    content_code += "}\n\n";
                                    
                                    
                                }
                                    break;
                                case BASE_TYPE_STRING:{
                                    
                                    std::string element_name = lang.string_type;
                                    std::string element_type_full = "<"+element_name+" *>";
                                    code += "@property (nonatomic, strong)"+type_name_dest+element_type_full+" *"+field.name+";\n\n";
                                    content_code += "- (" + type_name_dest + element_type_full + " *) " + field.name + " {\n\n";
                                    content_code += "    _"+field.name+" = [self fb_getStrings:"+NumToString(field.value.offset)+" origin:_"+field.name+"];\n\n";
                                    content_code += "    return _"+field.name+";\n\n";
                                    content_code += "}\n\n";
                                    
                                    
                                    
                                    content_code += "- (void) add_" + field.name + " {\n\n";
                                    content_code += "    [self fb_addStrings:_"+field.name+" voffset:"+NumToString(field.value.offset)+" offset:"+NumToString(bytesize+4)+"];\n\n";
                                    
                                    content_code += "    return ;\n\n";
                                    content_code += "}\n\n";
                                }
                                    break;
                                case BASE_TYPE_UNION:
                                    break;
                                    
                                default:{
                                    std::string element_name = "NSNumber";
                                    std::string element_type_full = "<"+element_name+" *>";
                                    std::string number_method = GenNumberMethod(field.value.type.element);
                                    std::string get_method = GenGetMethod(field.value.type.element);
                                    std::string number_type = GenNumberType(field.value.type.element);
                                    
                                    
                                    code += "@property (nonatomic, strong)"+type_name_dest+element_type_full+" *"+field.name+";\n\n";
                                    content_code += "- (" + type_name_dest + element_type_full + " *) " + field.name + " {\n\n";
                                    content_code += "    _"+field.name+" = [self fb_getNumbers:"+NumToString(field.value.offset)+" origin:_"+field.name+" type:"+number_type+"];\n\n";
                                    content_code += "    return _"+field.name+";\n\n";
                                    content_code += "}\n\n";
                                    
                                    
                                    
                                    content_code += "- (void) add_" + field.name + " {\n\n";
                                    content_code += "    [self fb_addNumbers:_"+field.name+" voffset:"+NumToString(field.value.offset)+" offset:"+NumToString(bytesize+4)+" type:"+number_type+"];\n\n";
                                    
                                    content_code += "    return ;\n\n";
                                    content_code += "}\n\n";
                                }
                                    break;
                            }
                            
                        }
                            break;
                        case BASE_TYPE_UNION:{
                            
                        }
                            break;
                        case BASE_TYPE_UTYPE:{
                            std::string getMethod = GenGetMethod(field.value.type.base_type);
                            type_name_dest = nameSpace(parser) + field.value.type.enum_def->name;
                            code += "@property (nonatomic, assign)"+type_name_dest+" "+field.name+";\n\n";
                            content_code += "- (" + type_name_dest + ") " + field.name + " {\n";
                            content_code += "    if (!_" + field.name + ") {\n";
                            
                            if (struct_def.fixed) {
                                
                                content_code += "        _"+field.name+" = [bb get"+getMethod+":"+NumToString(field.value.offset)+"+bb_pos];\n";
                                
                            }else{
                                
                                content_code += "        int o = [self fb_offset:" + NumToString(field.value.offset) + "];\n";
                                content_code += "        if (o != 0) {\n";
                                content_code += "            _"+field.name+" = [bb get"+getMethod+":o+bb_pos];\n";
                                content_code += "        }\n";
                                
                            }
                            
                            content_code += "    }\n";
                            content_code += "    return _"+field.name+";\n";
                            content_code += "}\n\n";
                        }
                            break;
                        default:
                        {
                            std::string getMethod = GenGetMethod(field.value.type.base_type);
                            
                            code += "@property (nonatomic, assign)"+type_name_dest+" "+field.name+";\n\n";
                            content_code += "- (" + type_name_dest + ") " + field.name + " {\n\n";
                            
                            if (struct_def.fixed) {
                                content_code += "    if (!_" + field.name + ") {\n\n";
                                content_code += "        _"+field.name+" = [bb get"+getMethod+":"+NumToString(field.value.offset)+"+bb_pos];\n\n";
                                content_code += "    }\n\n";
                            }else{
                                
                                content_code += "    _"+field.name+" = [self fb_get"+getMethod+":"+NumToString(field.value.offset)+" origin:_"+field.name+"];\n\n";
                                
                            }
                            
                            content_code += "    return _"+field.name+";\n\n";
                            content_code += "}\n\n";
                            
                            
                            
                            content_code += "- (void) add_"+field.name+" {\n\n";
                            if (struct_def.fixed) {
                                content_code += "    [self fb_set"+getMethod+":_"+field.name+" voffset:"+NumToString(field.value.offset)+" offset:"+NumToString(bytesize+4)+"];\n\n";
                            }else{
                                content_code += "    [self fb_add"+getMethod+":_"+field.name+" voffset:"+NumToString(field.value.offset)+" offset:"+NumToString(bytesize+4)+"];\n\n";
                            }
                            content_code += "    return ;\n\n";
                            content_code += "}\n\n";

                        }
                            break;
                    }
                    
                    bytesize += InlineSize(field.value.type);
                    
                }
                
                content_code += "- (instancetype)init{\n\n";
                content_code += "    if (self = [super init]) {\n\n";
                if (struct_def.fixed) {
                    content_code += "        bb_pos = 0;\n\n";
                    content_code += "        origin_size = "+NumToString(bytesize)+";\n\n";
                    content_code += "        bb = [[FBMutableData alloc]initWithLength:bb_pos + origin_size];\n\n";
                }else{
                    
                    content_code += "        bb_pos = "+NumToString(4+2+2+2+2*struct_def.fields.vec.size())+";\n\n";
                    content_code += "        origin_size = "+NumToString(4 + bytesize)+"+bb_pos;\n\n";
                    content_code += "        bb = [[FBMutableData alloc]initWithLength:origin_size];\n\n";
                    content_code += "        [bb setInt32:bb_pos offset:0];\n\n";
                    content_code += "        [bb setInt32:"+NumToString(2+2+2*struct_def.fields.vec.size())+" offset:bb_pos];\n\n";
                    content_code += "        [bb setInt16:"+NumToString(4+(struct_def.fields.vec.size())*2)+" offset:bb_pos-[bb getInt32:bb_pos]];\n\n";
                    content_code += "        [bb setInt16:"+NumToString(bytesize+4)+" offset:bb_pos-[bb getInt32:bb_pos]+2];\n\n";
                }
                content_code += "    }\n\n";
                content_code += "    return self;\n\n";
                content_code += "}\n\n";
                

                code += "@end\n";
                
                content_code += "@end\n";
                
                real_code += headercode +"\n\n"+ code;
            }

            
            ObjcGenerator(const Parser &parser, const std::string &path,
                         const std::string &file_name)
            : BaseGenerator(parser, path, file_name){};
            bool generate() {
                auto lang = language_parameters;
                std::string one_file_code;
                
                for (auto it = parser_.enums_.vec.begin(); it != parser_.enums_.vec.end();
                     ++it) {
                    std::string enumcode;
                    GenEnum(lang, parser_, **it, &enumcode);
                    if (parser_.opts.one_file) {
                        one_file_code += enumcode;
                    } else {
                        if (!SaveHeader(lang, parser_, nameSpace(parser_) + (**it).name, enumcode, path_, false,false))
                            return false;
                    }
                }
                
                for (auto it = parser_.structs_.vec.begin();
                     it != parser_.structs_.vec.end(); ++it) {
                    std::string declcode;
                    std::string contentcode;
                    GenClass(lang, parser_, **it, &declcode, &contentcode);
                    if (parser_.opts.one_file) {
                        one_file_code += declcode;
                    } else {
                        if (!SaveHeader(lang, parser_, nameSpace(parser_) + (**it).name, declcode,path_, true,
                                       false))
                            return false;
                        if (!SaveClass(lang, parser_, nameSpace(parser_) + (**it).name, contentcode,path_, true,
                                       false))
                            return false;
                    }
                }
                
                if (parser_.opts.one_file) {
                    return SaveClass(lang, parser_, file_name_, one_file_code, path_, true,
                                     true);
                }
                return true;
            }
            
        private:
            
        };
    }  // namespace objc
    
    bool GenerateObjc(const Parser &parser, const std::string &path,
                     const std::string &file_name) {
        objc::ObjcGenerator generator(parser, path, file_name);
        return generator.generate();
    }
}  // namespace flatbuffers
