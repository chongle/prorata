/****************************************************************************
** Meta object code from reading C++ file 'qwt_dial.h'
**
** Created: ??? ?? 25 15:38:52 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/qwt_dial.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwt_dial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QwtDial[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       6,   10, // properties
       2,   28, // enums/sets

 // properties: name, type, flags
      13,    8, 0x01095003,
      35,   31, 0x02095103,
      52,   45, 0x0009510b,
      69,   64, 0x0009510b,
      81,   74, 0x06095103,
      88,    8, 0x01095103,

 // enums: name, flags, count, data
      45, 0x0,    3,   36,
      64, 0x0,    2,   42,

 // enum data: key, value
      97, uint(QwtDial::Plain),
     103, uint(QwtDial::Raised),
     110, uint(QwtDial::Sunken),
     117, uint(QwtDial::RotateNeedle),
     130, uint(QwtDial::RotateScale),

       0        // eod
};

static const char qt_meta_stringdata_QwtDial[] = {
    "QwtDial\0bool\0visibleBackground\0int\0lineWidth\0Shadow\0frameShadow\0"
    "Mode\0mode\0double\0origin\0wrapping\0Plain\0Raised\0Sunken\0"
    "RotateNeedle\0RotateScale\0"
};

const QMetaObject QwtDial::staticMetaObject = {
    { &QwtAbstractSlider::staticMetaObject, qt_meta_stringdata_QwtDial,
      qt_meta_data_QwtDial, 0 }
};

const QMetaObject *QwtDial::metaObject() const
{
    return &staticMetaObject;
}

void *QwtDial::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QwtDial))
	return static_cast<void*>(const_cast<QwtDial*>(this));
    return QwtAbstractSlider::qt_metacast(_clname);
}

int QwtDial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtAbstractSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = hasVisibleBackground(); break;
        case 1: *reinterpret_cast< int*>(_v) = lineWidth(); break;
        case 2: *reinterpret_cast< Shadow*>(_v) = frameShadow(); break;
        case 3: *reinterpret_cast< Mode*>(_v) = mode(); break;
        case 4: *reinterpret_cast< double*>(_v) = origin(); break;
        case 5: *reinterpret_cast< bool*>(_v) = wrapping(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: showBackground(*reinterpret_cast< bool*>(_v)); break;
        case 1: setLineWidth(*reinterpret_cast< int*>(_v)); break;
        case 2: setFrameShadow(*reinterpret_cast< Shadow*>(_v)); break;
        case 3: setMode(*reinterpret_cast< Mode*>(_v)); break;
        case 4: setOrigin(*reinterpret_cast< double*>(_v)); break;
        case 5: setWrapping(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
