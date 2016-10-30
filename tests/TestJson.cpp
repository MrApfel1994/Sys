#include <catch.hpp>

#include <sstream>

#include "../Json.h"

namespace {
    const char json_example[] = "{"
                                "\t\"widget\": {\n"
                                "\t\t\"debug\": \"on\",\n"
                                "\t\t\"window\": {\n"
                                "\t\t\t\"title\": \"Sample Konfabulator Widget\",\n"
                                "\t\t\t\"name\": \"main_window\",\n"
                                "\t\t\t\"width\": 500,\n"
                                "\t\t\t\"height\": 500\n"
                                "\t\t},\n"
                                "\t\t\"image\": { \n"
                                "\t\t\t\"src\": \"Images/Sun.png\",\n"
                                "\t\t\t\"name\": \"sun1\",\n"
                                "\t\t\t\"hOffset\": 250,\n"
                                "\t\t\t\"vOffset\": 250,\n"
                                "\t\t\t\"alignment\": \"center\"\n"
                                "\t\t},\n"
                                "\t\t\"text\": {\n"
                                "\t\t\t\"data\": \"Click Here\",\n"
                                "\t\t\t\"size\": 36,\n"
                                "\t\t\t\"style\": \"bold\",\n"
                                "\t\t\t\"name\": \"text1\",\n"
                                "\t\t\t\"hOffset\": 250,\n"
                                "\t\t\t\"vOffset\": 100,\n"
                                "\t\t\t\"alignment\": \"center\",\n"
                                "\t\t\t\"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"\n"
                                "\t\t}\n"
                                "\t}"
                                "}";

    const char json_example2[] = "{\n"
                                "\t\"glossary\": {\n"
                                "\t\t\"title\": \"example glossary\",\n"
                                "\t\t\"GlossDiv\": {\n"
                                "\t\t\t\"title\": \"S\",\n"
                                "\t\t\t\"GlossList\": {\n"
                                "\t\t\t\t\"GlossEntry\": {\n"
                                "\t\t\t\t\t\"ID\": \"SGML\",\n"
                                "\t\t\t\t\t\"SortAs\": \"SGML\",\n"
                                "\t\t\t\t\t\"GlossTerm\": \"Standard Generalized Markup Language\",\n"
                                "\t\t\t\t\t\"Acronym\": \"SGML\",\n"
                                "\t\t\t\t\t\"Abbrev\": \"ISO 8879:1986\",\n"
                                "\t\t\t\t\t\"GlossDef\": {\n"
                                "\t\t\t\t\t\t\"para\": \"A meta-markup language, used to create markup languages such as DocBook.\",\n"
                                "\t\t\t\t\t\t\"GlossSeeAlso\": [\"GML\", \"XML\"]\n"
                                "\t\t\t\t\t},\n"
                                "\t\t\t\t\t\"GlossSee\": \"markup\"\n"
                                "\t\t\t\t}\n"
                                "\t\t\t}\n"
                                "\t\t}\n"
                                "\t}\n"
                                "}";

    const char json_example3[] = "{\n"
                                 "\t\"menu\": {\n"
                                 "\t\t\"id\": \"file\",\n"
                                 "\t\t\"value\": \"File\",\n"
                                 "\t\t\"popup\": {\n"
                                 "\t\t\t\"menuitem\": [\n"
                                 "\t\t\t\t{\"value\": \"New\", \"onclick\": \"CreateNewDoc()\"},\n"
                                 "\t\t\t\t{\"value\": \"Open\", \"onclick\": \"OpenDoc()\"},\n"
                                 "\t\t\t\t{\"value\": \"Close\", \"onclick\": \"CloseDoc()\"}\n"
                                 "\t\t\t]\n"
                                 "\t\t}\n"
                                 "\t}\n"
                                 "}";
}

TEST_CASE("Test types", "[Json]") {
    SECTION("JsNumber") {
        JsNumber n1(3.1568);
        std::stringstream ss;
        n1.Write(ss);
        JsNumber n2;
        REQUIRE(n2.Read(ss));
        REQUIRE(n2 == Approx(n1));
        n1 = 5;
        n2 = 5;
        REQUIRE(n2 == double(5));
        REQUIRE(n1 == n2);
    }

    SECTION("JsString") {
        JsString s1("qwe123");
        std::stringstream ss;
        s1.Write(ss);
        JsString s2;
        REQUIRE(s2.Read(ss));
        REQUIRE(s1 == s2);
        s2 = "asd111";
        REQUIRE(s2 == "asd111");
    }

    SECTION("JsArray") {
        JsArray a1;
        a1.Push(1);
        a1.Push(2);
        a1.Push("qwe123");
        REQUIRE(a1.Size() == 3);
        REQUIRE((JsNumber)a1[0] == Approx(1));
        REQUIRE((JsNumber)a1[1] == Approx(2));
        REQUIRE((JsString)a1[2] == "qwe123");
        std::stringstream ss;
        a1.Write(ss);
        JsArray a2;
        REQUIRE(a2.Read(ss));
        REQUIRE(a2.Size() == 3);
        REQUIRE((JsNumber)a2[0] == Approx(1));
        REQUIRE((JsNumber)a2[1] == Approx(2));
        REQUIRE((JsString)a2[2] == "qwe123");

        REQUIRE_THROWS(a2.at(3));

        // check equality
        JsArray a3, a4, a5;
        a3.Push("asdf");
        a3.Push("zxc");
        a4.Push("asdf");
        a4.Push("zxc");
        a5.Push("asdf");
        a5.Push("zxc1");
        REQUIRE(a3 == a4);
        REQUIRE(a3 != a5);
    }

    SECTION("JsObject") {
        JsObject obj;
        obj["123"] = 143;
        obj["asdf"] = "asdfsdf";
        obj["123"] = 46;
        REQUIRE(obj.Size() == 2);
        REQUIRE((JsNumber)obj["123"] == 46);
        REQUIRE((JsString)obj["asdf"] == "asdfsdf");
        std::stringstream ss;
        obj.Write(ss);
        JsObject _obj;
        REQUIRE(_obj.Read(ss));
        REQUIRE(_obj.Size() == 2);
        REQUIRE((JsNumber)_obj["123"] == Approx(46));
        REQUIRE((JsString)_obj["asdf"] == "asdfsdf");

        REQUIRE_THROWS(_obj.at("non exists"));

        // check equality
        JsObject obj1, obj2, obj3;
        obj1["123"] = 143;
        obj1["asdf"] = "asdfsdf";
        obj2["123"] = 143;
        obj2["asdf"] = "asdfsdf";
        obj3["123"] = 143;
        obj3["asdf"] = "asdfsdf1";
        REQUIRE(obj1 == obj2);
        REQUIRE(obj1 != obj3);
    }

    SECTION("JsLiteral") {
        JsLiteral lit(JS_TRUE);
        REQUIRE(lit.val == JS_TRUE);
        std::stringstream ss;
        lit.Write(ss);
        JsLiteral _lit(JS_NULL);
        REQUIRE(_lit.Read(ss));
        REQUIRE(_lit.val == JS_TRUE);

        // check equality
        JsLiteral lit1(JS_FALSE), lit2(JS_FALSE), lit3(JS_NULL);
        REQUIRE(lit1 == lit2);
        REQUIRE(lit1 != lit3);
    }

    SECTION("JsElement") {
        JsElement _el1(16);
        const JsElement &el1 = _el1;
        REQUIRE_NOTHROW((JsNumber)_el1);
        REQUIRE_NOTHROW((const JsNumber &)el1);
        REQUIRE_THROWS((JsString)_el1);
        REQUIRE_THROWS((const JsString &)el1);
        REQUIRE_THROWS((JsArray)_el1);
        REQUIRE_THROWS((const JsArray &)el1);
        REQUIRE_THROWS((JsObject)_el1);
        REQUIRE_THROWS((const JsObject &)el1);
        REQUIRE_THROWS((JsLiteral)_el1);
        REQUIRE_THROWS((const JsLiteral &)el1);

        JsElement _el2("my string");
        const JsElement &el2 = _el2;
        REQUIRE_NOTHROW((JsString)_el2);
        REQUIRE_NOTHROW((const JsString &)el2);
        REQUIRE_THROWS((JsNumber)_el2);
        REQUIRE_THROWS((const JsNumber &)el2);
        REQUIRE_THROWS((JsArray)el2);
        REQUIRE_THROWS((const JsArray &)el2);
        REQUIRE_THROWS((JsObject)el2);
        REQUIRE_THROWS((const JsObject &)el2);
        REQUIRE_THROWS((JsLiteral)el2);
        REQUIRE_THROWS((const JsLiteral &)el2);

        JsElement _el3(JS_ARRAY);
        const JsElement &el3 = _el3;
        REQUIRE_NOTHROW((JsArray)_el3);
        REQUIRE_NOTHROW((const JsArray &)el3);
        REQUIRE_THROWS((JsNumber)_el3);
        REQUIRE_THROWS((const JsNumber &)el3);
        REQUIRE_THROWS((JsString)_el3);
        REQUIRE_THROWS((const JsString &)el3);
        REQUIRE_THROWS((JsObject)_el3);
        REQUIRE_THROWS((const JsObject &)el3);
        REQUIRE_THROWS((JsLiteral)_el3);
        REQUIRE_THROWS((const JsLiteral &)el3);

        JsElement _el4(JS_OBJECT);
        const JsElement &el4 = _el4;
        REQUIRE_NOTHROW((JsObject)_el4);
        REQUIRE_NOTHROW((const JsObject &)el4);
        REQUIRE_THROWS((JsNumber)_el4);
        REQUIRE_THROWS((const JsNumber &)el4);
        REQUIRE_THROWS((JsString)_el4);
        REQUIRE_THROWS((const JsString &)el4);
        REQUIRE_THROWS((JsArray)_el4);
        REQUIRE_THROWS((const JsArray &)el4);
        REQUIRE_THROWS((JsLiteral)_el4);
        REQUIRE_THROWS((const JsLiteral &)el4);

        JsElement _el5(JS_NULL);
        const JsElement &el5 = _el5;
        REQUIRE_NOTHROW((JsLiteral)_el5);
        REQUIRE_NOTHROW((const JsLiteral &)el5);
        REQUIRE_THROWS((JsNumber)_el5);
        REQUIRE_THROWS((const JsNumber &)el5);
        REQUIRE_THROWS((JsString)_el5);
        REQUIRE_THROWS((const JsString &)el5);
        REQUIRE_THROWS((JsArray)_el5);
        REQUIRE_THROWS((const JsArray &)el5);
        REQUIRE_THROWS((JsObject)_el5);
        REQUIRE_THROWS((const JsObject &)el5);
    }
}

TEST_CASE("Complex test1", "[Json]") {
    bool flag = true;
    JsElement el(JS_NULL);
    std::stringstream ss(json_example);
    REQUIRE(el.Read(ss));

AGAIN:
    JsObject &root = (JsObject &)el;
    REQUIRE(root.Size() == 1);
    JsObject &widget = (JsObject &)root["widget"];
    REQUIRE(widget.Size() == 4);
    REQUIRE(widget["debug"] == "on");
    JsObject &window = (JsObject &)widget["window"];
    REQUIRE(window.Size() == 4);
    REQUIRE(window["title"] == "Sample Konfabulator Widget");
    REQUIRE(window["name"] == "main_window");
    REQUIRE(window["width"] == 500);
    REQUIRE(window["height"] == 500);
    JsObject &image = (JsObject &)widget["image"];
    REQUIRE(image.Size() == 5);
    REQUIRE(image["src"] == "Images/Sun.png");
    REQUIRE(image["name"] == "sun1");
    REQUIRE(image["hOffset"] == 250);
    REQUIRE(image["vOffset"] == 250);
    REQUIRE(image["alignment"] == "center");
    JsObject &text = (JsObject &)widget["text"];
    REQUIRE(text.Size() == 8);
    REQUIRE(text["data"] == "Click Here");
    REQUIRE(text["size"] == 36);
    REQUIRE(text["style"] == "bold");
    REQUIRE(text["name"] == "text1");
    REQUIRE(text["hOffset"] == 250);
    REQUIRE(text["vOffset"] == 100);
    REQUIRE(text["alignment"] == "center");
    REQUIRE(text["onMouseUp"] == "sun1.opacity = (sun1.opacity / 100) * 90;");

    if (flag) {
        flag = false;
        ss.clear();
        ss.seekg(0);
        root.Write(ss);
        goto AGAIN;
    }
}

TEST_CASE("Complex test2", "[Json]") {
    bool flag = true;
    JsElement el(JS_NULL);
    std::stringstream ss(json_example2);
    REQUIRE(el.Read(ss));

AGAIN:
    JsObject &root = (JsObject &)el;
    REQUIRE(root.Size() == 1);
    JsObject &glossary = (JsObject &)root["glossary"];
    REQUIRE(glossary.Size() == 2);
    REQUIRE(glossary["title"] == "example glossary");
    JsObject &gloss_div = (JsObject &)glossary["GlossDiv"];
    REQUIRE(gloss_div.Size() == 2);
    REQUIRE(gloss_div["title"] == "S");
    JsObject &gloss_list = (JsObject &)gloss_div["GlossList"];
    REQUIRE(gloss_list.Size() == 1);
    JsObject &gloss_entry = (JsObject &)gloss_list["GlossEntry"];
    REQUIRE(gloss_entry.Size() == 7);
    REQUIRE(gloss_entry["ID"] == "SGML");
    REQUIRE(gloss_entry["SortAs"] == "SGML");
    REQUIRE(gloss_entry["GlossTerm"] == "Standard Generalized Markup Language");
    REQUIRE(gloss_entry["Acronym"] == "SGML");
    REQUIRE(gloss_entry["Abbrev"] == "ISO 8879:1986");
    REQUIRE(gloss_entry["GlossSee"] == "markup");
    JsObject &gloss_def = (JsObject &)gloss_entry["GlossDef"];
    REQUIRE(gloss_def.Size() == 2);
    REQUIRE(gloss_def["para"] == "A meta-markup language, used to create markup languages such as DocBook.");
    JsArray &gloss_see_also = (JsArray &)gloss_def["GlossSeeAlso"];
    REQUIRE(gloss_see_also.Size() == 2);
    REQUIRE(gloss_see_also[0] == "GML");
    REQUIRE(gloss_see_also[1] == "XML");

    if (flag) {
        flag = false;
        ss.clear();
        ss.seekg(0);
        root.Write(ss);
        goto AGAIN;
    }
}

TEST_CASE("Complex test3", "[Json]") {
    bool flag = true;
    JsElement el(JS_NULL);
    std::stringstream ss(json_example3);

AGAIN:
    REQUIRE(el.Read(ss));

    JsObject &root = (JsObject &)el;
    REQUIRE(root.Size() == 1);
    JsObject &menu = (JsObject &)root["menu"];
    REQUIRE(menu.Size() == 3);
    REQUIRE(menu["id"] == "file");
    REQUIRE(menu["value"] == "File");
    JsObject &popup = (JsObject &)menu["popup"];
    REQUIRE(popup.Size() == 1);
    JsArray &menuitem = (JsArray &)popup["menuitem"];
    REQUIRE(menuitem.Size() == 3);
    JsObject &_0 = (JsObject &)menuitem[0];
    REQUIRE(_0.Size() == 2);
    JsObject &_1 = (JsObject &)menuitem[1];
    REQUIRE(_1.Size() == 2);
    JsObject &_2 = (JsObject &)menuitem[2];
    REQUIRE(_2.Size() == 2);
    REQUIRE(_0["value"] == "New");
    REQUIRE(_0["onclick"] == "CreateNewDoc()");
    REQUIRE(_1["value"] == "Open");
    REQUIRE(_1["onclick"] == "OpenDoc()");
    REQUIRE(_2["value"] == "Close");
    REQUIRE(_2["onclick"] == "CloseDoc()");

    if (flag) {
        flag = false;
        ss.clear();
        ss.seekg(0);
        root.Write(ss);
        std::string str = ss.str();
        goto AGAIN;
    }
}