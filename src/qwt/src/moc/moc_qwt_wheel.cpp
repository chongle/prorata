/****************************************************************************
** Meta object code from reading C++ file 'qwt_wheel.h'
**
** Created: ??? ?? 25 15:38:58 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/qwt_wheel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_wheel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QwtWheel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   10, // properties
       0,    0, // enums/sets

 // properties: name, type, flags
      16,    9, 0x06095103,
      27,    9, 0x06095103,
      41,   37, 0x02095103,
      49,   37, 0x02095103,
      64,    9, 0x06095103,

       0        // eod
};

static const char qt_meta_stringdata_QwtWheel[] = {
    "QwtWheel\0double\0totalAngle\0viewAngle\0int\0tickCnt\0internalBorder\0"
    "mass\0"
};

const QMetaObject QwtWheel::staticMetaObject = {
    { &QwtAbstractSlider::staticMetaObject, qt_meta_stringdata_QwtWheel,
      qt_meta_data_QwtWheel, 0 }
};

const QMetaObject *QwtWheel::metaObject() const
{
    return &staticMetaObject;
}

void *QwtWheel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtWheel))
	return static_cast<void*>(const_cast<QwtWheel*>(this));
    return QwtAbstractSlider::qt_metacast(_clname);
}

int QwtWheel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtAbstractSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = totalAngle(); break;
        case 1: *reinterpret_cast< double*>(_v) = viewAngle(); break;
        case 2: *reinterpret_cast< int*>(_v) = tickCnt(); break;
        case 3: *reinterpret_cast< int*>(_v) = internalBorder(); break;
        case 4: *reinterpret_cast< double*>(_v) = mass(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setTotalAngle(*reinterpret_cast< double*>(_v)); break;
        case 1: setViewAngle(*reinterpret_cast< double*>(_v)); break;
        case 2: setTickCnt(*reinterpret_cast< int*>(_v)); break;
        case 3: setInternalBorder(*reinterpret_cast< int*>(_v)); break;
        case 4: setMass(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
