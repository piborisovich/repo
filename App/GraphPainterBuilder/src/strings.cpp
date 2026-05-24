/*!
 * \file strings.hpp
 *
 * \brief  Строковые константы. Определения
 */

#include "strings.hpp"

namespace Strings
{

const char *APPLICATION_NAME = "Graph Painter Builder";

const char *UNDO_TEXT = "↩ Undo";
const char *REDO_TEXT = "↪ Redo";

const char *TOOL_TEXT = "<b>Tool:</b>";
const char *BRUSH_SIZE_TEXT = "<b>Brush size/border:</b>";
const char *PALETTE_TEXT = "<b>Palette:</b>";

const char *IMPORT_TEXT = "Import image";
const char *EXPORT_TEXT = "Export to image";
const char *CLEAR_CANVAS_TEXT = "Clear the canvas";

const char *LAYERS_TITLE = "<b>Layers (Z-Index):</b>";
const char *LAYER_NAME_1 = "Layer 1 (Bottom)";
const char *LAYER_NAME_2 = "Layer 2 (Middle)";
const char *LAYER_NAME_3 = "Layer 3 (Top)";

const char *ADD_LAYER_TEXT = "+ Add layer";
const char *DELETE_LAYER_TEXT = "- Remove current layer";

const char *SELECT_COLOR_TEXT = "Choose a color";

const char *NEW_LAYER_DIALOG_TITLE = "New layer";
const char *NEW_LAYER_DIALOG_LABEL = "Layer name";

const char *REMOVE_LAYER_MSG_BOX_TITLE = "Layer removal";
const char *REMOVE_LAYER_MSG_BOX_QUESTION_TEMPLATE = "Do you want to delete \"%1\" "
                                                     "and everything on it? "
                                                     "(This action will clear the "
                                                     "undo history.)";
const char *REMOVE_LAYER_ERROR_MSG_BOX_TITLE = "Error";
const char *REMOVE_LAYER_ERROR_MSG_BOX_MESSAGE = "You can't delete a single layer";

const char *CLEAR_CANVAS_MSG_BOX_TITLE = "Clearing";
const char *CLEAR_CANVAS_MSG_BOX_QUESTION = "Are you sure you want "
                                            "to clear the canvas? "
                                            "(Undo history will be reset)";

const char *OPEN_IMAGE_DIALOIG_TITLE = "Image import";
const char *SAVE_IMAGE_DIALOIG_TITLE = "Image export";

const char *SAVE_SUCCESS_MSG_BOX_TITLE = "Success";
const char *SAVE_SUCCESS_MSG_BOX_MESSAGE = "The image was saved successfully!";
} //Strings
