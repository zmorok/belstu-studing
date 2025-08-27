from telegram import Update
from telegram.ext import ApplicationBuilder, CommandHandler, MessageHandler, filters, ContextTypes

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        'Привет! Я бот. Вот что я умею:\n'
        '- /start — начать общение\n'
        '- /help — справка по функциям\n'
        '- Реагирую на слово \'привет\'\n'
        '- Принимаю фото, файлы и стикеры\n'
        '- Всё остальное — говорю, что не понимаю.'
    )

async def help_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        'Справка:\n'
        '/start — приветствие\n'
        '/help — список команд\n'
        'Скажи \'привет\', пришли фото, файл или стикер — я отреагирую.'
    )

async def handle_text(update: Update, context: ContextTypes.DEFAULT_TYPE):
    user_text = update.message.text.lower()
    if 'привет' in user_text:
        await update.message.reply_text('Приветствую!')
    else:
        await update.message.reply_text('Я не понимаю')

async def handle_photo(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text('Классное фото!')

async def handle_document(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text('Файл получен.')

async def handle_sticker(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text('О, прикольный стикер!')

async def handle_unknown(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text('Я не понимаю')

if __name__ == '__main__':
    import os
    from dotenv import load_dotenv

    load_dotenv()
    TOKEN = os.getenv('TOKEN')

    app = ApplicationBuilder().token(TOKEN).build()


    app.add_handler(CommandHandler('start', start))
    app.add_handler(CommandHandler('help', help_command))


    app.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, handle_text))
    app.add_handler(MessageHandler(filters.PHOTO, handle_photo))
    app.add_handler(MessageHandler(filters.Document.ALL, handle_document))
    app.add_handler(MessageHandler(filters.Sticker.ALL, handle_sticker))


    app.add_handler(MessageHandler(filters.ALL, handle_unknown))

    print('Бот1 запущен...')
    app.run_polling()