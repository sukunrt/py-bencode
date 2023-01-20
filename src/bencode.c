#include "sds.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Python.h"

sds encode_obj(PyObject *o);

sds add_int(sds s, int x)
{
        return sdscatfmt(s, "i%ie", x);
}

sds add_str(sds s, const char *ss)
{
        return sdscatfmt(s, "%u:%s", strlen(ss), ss);
}

sds encode_list(PyListObject *li)
{
        sds res = sdsnew("l");
        int i = 0;
        for (i = 0; i < PyList_Size(li); i++)
        {
                sds r1 = encode_obj(PyList_GET_ITEM(li, i));
                res = sdscat(res, r1);
                sdsfree(r1);
        }
        res = sdscat(res, "e");
        return res;
}

sds encode_dict(PyDictObject *d)
{
        sds res = sdsnew("d");
        PyListObject *li = PyDict_Items(d);
        PyList_Sort(li);
        int i = 0;
        for (i = 0; i < PyList_Size(li); i++)
        {
                PyTupleObject *b = PyList_GET_ITEM(li, i);
                sds r1 = encode_obj(PyTuple_GET_ITEM(b, 0));
                sds r2 = encode_obj(PyTuple_GET_ITEM(b, 1));
                res = sdscat(res, r1);
                res = sdscat(res, r2);
                sdsfree(r1);
                sdsfree(r2);
        }
        res = sdscat(res, "e");
        return res;
}

sds encode_obj(PyObject *o)
{
        sds res = sdsnew("");
        sds r1;
        if (PyList_Check(o) != 0)
        {
                r1 = encode_list((PyListObject *)o);
        }
        else if (PyDict_Check(o) != 0)
        {
                r1 = encode_dict((PyDictObject *)o);
        }
        else if (PyLong_Check(o) != 0)
        {
                r1 = add_int(sdsnew(""), _PyLong_AsInt(o));
        }
        else
        {
                r1 = add_str(sdsnew(""), PyUnicode_AsUTF8AndSize(o, NULL));
        }
        res = sdscat(res, r1);
        sdsfree(r1);
        return res;
}

static PyObject *method_dumps(PyObject *self, PyObject *args)
{
        sds res = encode_obj(args);
        PyObject *repsonse = Py_BuildValue("s", res);
}

static PyMethodDef Methods[] = {
    {"dumps", method_dumps, METH_O, "convert to bencoded string"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef bencodemodule = {
    PyModuleDef_HEAD_INIT,
    "bencode",
    "encoding and decoding to bencoded str",
    -1,
    Methods};

PyMODINIT_FUNC PyInit_bencode(void)
{
        return PyModule_Create(&bencodemodule);
}
