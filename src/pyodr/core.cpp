#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <odr/document.hpp>
#include <odr/document_element.hpp>
#include <odr/file.hpp>
#include <odr/filesystem.hpp>
#include <odr/html.hpp>
#include <odr/open_document_reader.hpp>

namespace py = pybind11;

PYBIND11_MODULE(core, m) {
  {
    m.attr("__version__") = odr::OpenDocumentReader::version();
    m.attr("version") = odr::OpenDocumentReader::version();
    m.attr("commit") = odr::OpenDocumentReader::commit();
  }

  {
    py::enum_<odr::FileType>(m, "FileType")
        .value("unknown", odr::FileType::unknown)
        .value("opendocument_text", odr::FileType::opendocument_text)
        .value("opendocument_presentation",
               odr::FileType::opendocument_presentation)
        .value("opendocument_spreadsheet",
               odr::FileType::opendocument_spreadsheet)
        .value("opendocument_graphics", odr::FileType::opendocument_graphics)
        .value("office_open_xml_document",
               odr::FileType::office_open_xml_document)
        .value("office_open_xml_presentation",
               odr::FileType::office_open_xml_presentation)
        .value("office_open_xml_workbook",
               odr::FileType::office_open_xml_workbook)
        .value("office_open_xml_encrypted",
               odr::FileType::office_open_xml_encrypted)
        .value("legacy_word_document", odr::FileType::legacy_word_document)
        .value("legacy_powerpoint_presentation",
               odr::FileType::legacy_powerpoint_presentation)
        .value("legacy_excel_worksheets",
               odr::FileType::legacy_excel_worksheets)
        .value("word_perfect", odr::FileType::word_perfect)
        .value("rich_text_format", odr::FileType::rich_text_format)
        .value("portable_document_format",
               odr::FileType::portable_document_format)
        .value("text_file", odr::FileType::text_file)
        .value("comma_separated_values", odr::FileType::comma_separated_values)
        .value("javascript_object_notation",
               odr::FileType::javascript_object_notation)
        .value("markdown", odr::FileType::markdown)
        .value("zip", odr::FileType::zip)
        .value("compound_file_binary_format",
               odr::FileType::compound_file_binary_format)
        .value("portable_network_graphics",
               odr::FileType::portable_network_graphics)
        .value("graphics_interchange_format",
               odr::FileType::graphics_interchange_format)
        .value("jpeg", odr::FileType::jpeg)
        .value("bitmap_image_file", odr::FileType::bitmap_image_file)
        .value("starview_metafile", odr::FileType::starview_metafile);

    py::enum_<odr::FileCategory>(m, "FileCategory")
        .value("unknown", odr::FileCategory::unknown)
        .value("text", odr::FileCategory::text)
        .value("image", odr::FileCategory::image)
        .value("archive", odr::FileCategory::archive)
        .value("document", odr::FileCategory::document);

    py::enum_<odr::FileLocation>(m, "FileLocation")
        .value("memory", odr::FileLocation::memory)
        .value("disk", odr::FileLocation::disk);

    py::enum_<odr::EncryptionState>(m, "EncryptionState")
        .value("unknown", odr::EncryptionState::unknown)
        .value("not_encrypted", odr::EncryptionState::not_encrypted)
        .value("encrypted", odr::EncryptionState::encrypted)
        .value("decrypted", odr::EncryptionState::decrypted);

    py::class_<odr::FileMeta>(m, "FileMeta")
        .def_readwrite("type", &odr::FileMeta::type)
        .def_readwrite("password_encrypted", &odr::FileMeta::password_encrypted)
        .def_readwrite("document_meta", &odr::FileMeta::document_meta);

    py::class_<odr::DocumentMeta>(m, "DocumentMeta")
        .def_readwrite("document_type", &odr::DocumentMeta::document_type)
        .def_readwrite("entry_count", &odr::DocumentMeta::entry_count);

    py::class_<odr::File>(m, "File")
        .def(py::init<const std::string &>())
        .def("is_valid", &odr::File::operator bool)
        .def("location", &odr::File::location)
        .def("size", &odr::File::size);

    py::class_<odr::DecodedFile>(m, "DecodedFile")
        .def(py::init<const odr::File &>())
        .def(py::init<const std::string &>())
        .def("is_valid", &odr::DecodedFile::operator bool)
        .def("file_type", &odr::DecodedFile::file_type)
        .def("file_category", &odr::DecodedFile::file_category)
        .def("file_meta", &odr::DecodedFile::file_meta)
        .def("file", &odr::DecodedFile::file)
        .def("is_text_file", &odr::DecodedFile::is_text_file)
        .def("is_image_file", &odr::DecodedFile::is_image_file)
        .def("is_archive_file", &odr::DecodedFile::is_archive_file)
        .def("is_document_file", &odr::DecodedFile::is_document_file)
        .def("is_pdf_file", &odr::DecodedFile::is_pdf_file)
        .def("text_file", &odr::DecodedFile::text_file)
        .def("image_file", &odr::DecodedFile::image_file)
        .def("archive_file", &odr::DecodedFile::archive_file)
        .def("document_file", &odr::DecodedFile::document_file)
        .def("pdf_file", &odr::DecodedFile::pdf_file);

    py::class_<odr::TextFile>(m, "TextFile")
        .def("is_valid", &odr::TextFile::operator bool)
        .def("charset", &odr::TextFile::charset)
        .def("text", &odr::TextFile::text);

    py::class_<odr::ImageFile>(m, "ImageFile")
        .def("is_valid", &odr::ImageFile::operator bool);

    py::class_<odr::ArchiveFile>(m, "ArchiveFile")
        .def("is_valid", &odr::ArchiveFile::operator bool);

    py::class_<odr::DocumentFile>(m, "DocumentFile")
        .def("is_valid", &odr::DocumentFile::operator bool)
        .def("password_encrypted", &odr::DocumentFile::password_encrypted)
        .def("encryption_state", &odr::DocumentFile::encryption_state)
        .def("decrypt", &odr::DocumentFile::decrypt)
        .def("document_type", &odr::DocumentFile::document_type)
        .def("document_meta", &odr::DocumentFile::document_meta)
        .def("document", &odr::DocumentFile::document);

    py::class_<odr::PdfFile>(m, "PdfFile")
        .def("is_valid", &odr::PdfFile::operator bool);
  }

  {
    py::class_<odr::Filesystem>(m, "Filesystem")
        .def("is_valid", &odr::Filesystem::operator bool)
        .def("exists", &odr::Filesystem::exists)
        .def("is_file", &odr::Filesystem::is_file)
        .def("is_directory", &odr::Filesystem::is_directory)
        .def("file_walker", &odr::Filesystem::file_walker)
        .def("open", &odr::Filesystem::open);

    py::class_<odr::FileWalker>(m, "FileWalker")
        .def("is_valid", &odr::FileWalker::operator bool)
        .def("next", &odr::FileWalker::next);
  }

  {
    py::enum_<odr::DocumentType>(m, "DocumentType")
        .value("unknown", odr::DocumentType::unknown)
        .value("text", odr::DocumentType::text)
        .value("presentation", odr::DocumentType::presentation)
        .value("spreadsheet", odr::DocumentType::spreadsheet)
        .value("drawing", odr::DocumentType::drawing);

    py::class_<odr::Document>(m, "Document")
        .def("editable", &odr::Document::editable)
        .def("savable", &odr::Document::savable, py::arg("encrypted") = false)
        .def("save", py::overload_cast<const std::string &>(
                         &odr::Document::save, py::const_))
        .def("file_type", &odr::Document::file_type)
        .def("document_type", &odr::Document::document_type)
        .def("root_element", &odr::Document::root_element)
        .def("files", &odr::Document::files);
  }

  {
    py::enum_<odr::ElementType>(m, "ElementType")
        .value("none", odr::ElementType::none)
        .value("root", odr::ElementType::root)
        .value("slide", odr::ElementType::slide)
        .value("sheet", odr::ElementType::sheet)
        .value("page", odr::ElementType::page)
        .value("master_page", odr::ElementType::master_page)
        .value("text", odr::ElementType::text)
        .value("line_break", odr::ElementType::line_break)
        .value("page_break", odr::ElementType::page_break)
        .value("paragraph", odr::ElementType::paragraph)
        .value("span", odr::ElementType::span)
        .value("link", odr::ElementType::link)
        .value("bookmark", odr::ElementType::bookmark)
        .value("list", odr::ElementType::list)
        .value("list_item", odr::ElementType::list_item)
        .value("table", odr::ElementType::table)
        .value("table_column", odr::ElementType::table_column)
        .value("table_row", odr::ElementType::table_row)
        .value("table_cell", odr::ElementType::table_cell)
        .value("frame", odr::ElementType::frame)
        .value("image", odr::ElementType::image)
        .value("rect", odr::ElementType::rect)
        .value("line", odr::ElementType::line)
        .value("circle", odr::ElementType::circle)
        .value("custom_shape", odr::ElementType::custom_shape)
        .value("group", odr::ElementType::group);

    py::enum_<odr::AnchorType>(m, "AnchorType")
        .value("as_char", odr::AnchorType::as_char)
        .value("at_char", odr::AnchorType::at_char)
        .value("at_frame", odr::AnchorType::at_frame)
        .value("at_page", odr::AnchorType::at_page)
        .value("at_paragraph", odr::AnchorType::at_paragraph);

    py::enum_<odr::ValueType>(m, "ValueType")
        .value("unknown", odr::ValueType::unknown)
        .value("string", odr::ValueType::string)
        .value("float_number", odr::ValueType::float_number);

    py::class_<odr::Element>(m, "Element")
        .def("is_valid", &odr::Element::operator bool)
        .def("type", &odr::Element::type)
        .def("parent", &odr::Element::parent)
        .def("first_child", &odr::Element::first_child)
        .def("previous_sibling", &odr::Element::previous_sibling)
        .def("next_sibling", &odr::Element::next_sibling)
        .def("is_editable", &odr::Element::is_editable);
  }

  {
    py::enum_<odr::HtmlTableGridlines>(m, "HtmlTableGridlines")
        .value("none", odr::HtmlTableGridlines::none)
        .value("soft", odr::HtmlTableGridlines::soft)
        .value("hard", odr::HtmlTableGridlines::hard);

    py::class_<odr::HtmlConfig>(m, "HtmlConfig")
        .def(py::init<>())
        .def_readwrite("compact_presentation",
                       &odr::HtmlConfig::compact_presentation)
        .def_readwrite("compact_spreadsheet",
                       &odr::HtmlConfig::compact_spreadsheet)
        .def_readwrite("compact_drawing", &odr::HtmlConfig::compact_drawing)
        .def_readwrite("text_document_output_file_name",
                       &odr::HtmlConfig::text_document_output_file_name)
        .def_readwrite("presentation_output_file_name",
                       &odr::HtmlConfig::presentation_output_file_name)
        .def_readwrite("spreadsheet_output_file_name",
                       &odr::HtmlConfig::spreadsheet_output_file_name)
        .def_readwrite("drawing_output_file_name",
                       &odr::HtmlConfig::drawing_output_file_name)
        .def_readwrite("slide_output_file_name",
                       &odr::HtmlConfig::slide_output_file_name)
        .def_readwrite("sheet_output_file_name",
                       &odr::HtmlConfig::sheet_output_file_name)
        .def_readwrite("page_output_file_name",
                       &odr::HtmlConfig::page_output_file_name)
        .def_readwrite("embed_resources", &odr::HtmlConfig::embed_resources)
        .def_readwrite("external_resource_path",
                       &odr::HtmlConfig::external_resource_path)
        .def_readwrite("relative_resource_paths",
                       &odr::HtmlConfig::relative_resource_paths)
        .def_readwrite("editable", &odr::HtmlConfig::editable)
        .def_readwrite("text_document_margin",
                       &odr::HtmlConfig::text_document_margin)
        .def_readwrite("spreadsheet_limit", &odr::HtmlConfig::spreadsheet_limit)
        .def_readwrite("spreadsheet_limit_by_content",
                       &odr::HtmlConfig::spreadsheet_limit_by_content)
        .def_readwrite("spreadsheet_gridlines",
                       &odr::HtmlConfig::spreadsheet_gridlines)
        .def_readwrite("format_html", &odr::HtmlConfig::format_html)
        .def_readwrite("html_indent", &odr::HtmlConfig::html_indent);

    py::class_<odr::Html>(m, "Html")
        .def("file_type", &odr::Html::file_type)
        .def("pages", &odr::Html::pages)
        .def("edit", &odr::Html::edit)
        .def("save", &odr::Html::save);

    py::class_<odr::HtmlPage>(m, "HtmlPage")
        .def_readwrite("name", &odr::HtmlPage::name)
        .def_readwrite("path", &odr::HtmlPage::path);
  }

  {
    m.def("open", &odr::OpenDocumentReader::open);

    m.def("copy_resources", &odr::OpenDocumentReader::copy_resources);
  }

  {
    auto m_html = m.def_submodule("html");

    m_html.def(
        "translate",
        py::overload_cast<const odr::DecodedFile &, const std::string &,
                          const odr::HtmlConfig &>(&odr::html::translate));
  }
}
