#ifndef TABLEDELEGATES_H
#define TABLEDELEGATES_H
#include <QObject>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QDateEdit>
#include <QAbstractItemModel>
#include <QLabel>
#include <QBuffer>
#include <QPixmap>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QFileDialog>
class ComboBoxDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate(QObject* parent=nullptr):QStyledItemDelegate(parent){}
    // 设置下拉菜单的选项
    void setItems(const QStringList& items) {
        m_items = items; // 设置下拉菜单的选项
    }
    // 创建编辑器
    QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const override
    {
        Q_UNUSED(option);
        Q_UNUSED(index);

        QComboBox* editor= new QComboBox(parent);
        editor->addItems(m_items); // 添加选项到下单菜单
        return editor;
    }
    // 设置模型数据
    // void setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex& index) const override
    // {
    //     QComboBox* comboBox = static_cast<QComboBox*>(editor);
    //     QString value = comboBox->currentText();
    //     model->setData(index,value,Qt::EditRole);
    // }
private:
    QStringList m_items; //存储下拉下单的选项
};

class DateEditDelegate : public QStyledItemDelegate {
public:
    explicit DateEditDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override
    {
        QDateEdit* editor = new QDateEdit(parent);
        editor->setDisplayFormat("yyyy-MM-dd"); // 设置日期格式
        editor->setCalendarPopup(true); // 启用日历弹出
        return editor;
    }
    // void setModelData(QWidget* editor ,QAbstractItemModel* model,const QModelIndex& index) const override
    // {
    //     QDateEdit* dateEdit = qobject_cast<QDateEdit*>(editor);
    //     if (dateEdit)
    //         model->setData(index,dateEdit->date().toString("yyyy-MM-dd"),Qt::EditRole);
    // }
};

class ImageDelegate : public QStyledItemDelegate{
public:
    explicit ImageDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override
    {
        Q_UNUSED(option);
        Q_UNUSED(index);
        QLabel* editor = new QLabel(parent);
        return editor;
    }
    void setModelData(QWidget* editor,QAbstractItemModel* model,const QModelIndex& index) const override{
        QLabel* label = qobject_cast<QLabel*>(editor);
        if(label) {
            QByteArray imageData;
            QPixmap pixmap = label->pixmap(); // 直接获取 QPixmap 对象
            if (!pixmap.isNull()) {
                QBuffer buffer(&imageData);
                buffer.open(QIODevice::WriteOnly);
                pixmap.save(&buffer,"PNG"); // 将图片保存为PNG格式的二进制数据
            }
            model->setData(index,imageData,Qt::UserRole); // 将二进制数据保存到模型中
        }
    }
    void paint(QPainter* painter,const QStyleOptionViewItem& option,const QModelIndex& index) const override
    {
        // 从模型中获取二进制图片数据
        QByteArray imageData = index.data(Qt::UserRole).toByteArray();
        if (imageData.isEmpty())
        {
            QStyledItemDelegate::paint(painter,option,index);
            return ;
        }
        QPixmap pixmap; // 将二进制数据转换为 QPixmap
        pixmap.loadFromData(imageData);
        if (pixmap.isNull()) {
            QStyledItemDelegate::paint(painter,option,index);
            return ;
        }
        // 绘制图片
        QRect rect = option.rect;
        QPixmap scaledPixmap = pixmap.scaled(100,100,Qt::KeepAspectRatio); // 缩放图片
        painter->drawPixmap(rect,scaledPixmap);
    }
    bool editorEvent(QEvent* event,QAbstractItemModel* model,const QStyleOptionViewItem& option,
                                                            const QModelIndex& index) override
    {
        if(event->type()==QEvent::MouseButtonDblClick) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                // 弹出文件对话框选择新图片
                QString imagePath = QFileDialog::getOpenFileName(
                    nullptr,"选择图片","","图片文件（*.png *.jpg *.bmp）"
                );
                if(!imagePath.isEmpty()) { // 如果用户选择了有效的图片路径
                    QFile file(imagePath);
                    if(file.open(QIODevice::ReadOnly)) {
                        QByteArray imageData = file.readAll();
                        file.close();
                        model->setData(index,imageData,Qt::UserRole); // 将二进制数据保存到模型中
                    }
                }
                // 如果用户取消选择，保留原来的图片
                return true; // 事件已处理
            }
        }
        return QStyledItemDelegate::editorEvent(event,model,option,index);
    }
};

#endif // TABLEDELEGATES_H
